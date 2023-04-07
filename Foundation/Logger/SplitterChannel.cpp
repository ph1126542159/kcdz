#include "Logger/SplitterChannel.h"
#include "Logger/LoggingRegistry.h"
#include <assert.h>
#include <QStringList>
namespace PH {
SplitterChannel::SplitterChannel()
{
}


SplitterChannel::~SplitterChannel()
{
    try
    {
        close();
    }
    catch (...)
    {
    }
}


void SplitterChannel::addChannel(Channel::Ptr pChannel)
{
    assert( pChannel.get()!=nullptr);

    std::unique_lock<std::mutex> lock(_mutex);
    _channels.push_back(pChannel);
}


void SplitterChannel::removeChannel(Channel::Ptr pChannel)
{
    std::unique_lock<std::mutex> lock(_mutex);

    for (ChannelVec::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == pChannel)
        {
            _channels.erase(it);
            break;
        }
    }
}


void SplitterChannel::setProperty(const std::string& name, const std::string& value)
{
    if (name.compare(0, 7, "channel") == 0)
    {
        QStringList tokenizer=QString(value.c_str()).split(",;");
        foreach(const auto& it,tokenizer)
        {
            addChannel(LoggingRegistry::defaultRegistry().channelForName(it.toStdString()));
        }
    }
    else Channel::setProperty(name, value);
}


void SplitterChannel::log(const Message& msg)
{
    std::unique_lock<std::mutex> lock(_mutex);

    for (auto& p: _channels)
    {
        p->log(msg);
    }
}


void SplitterChannel::close()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _channels.clear();
}


int SplitterChannel::count()
{
    std::unique_lock<std::mutex> lock(_mutex);

    return (int) _channels.size();
}
}
