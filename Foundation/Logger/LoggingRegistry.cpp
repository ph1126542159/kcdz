#include "LoggingRegistry.h"
namespace PH {
LoggingRegistry::LoggingRegistry()
{
}


LoggingRegistry::~LoggingRegistry()
{
}


Channel::Ptr LoggingRegistry::channelForName(const std::string& name) const
{
    std::lock_guard<std::mutex> lock(_mutex);

    ChannelMap::const_iterator it = _channelMap.find(name);
    if (it != _channelMap.end())
        return it->second;
    else
        throw PH::Exception("Not Found Exception logging channel"+name);
}


Formatter::Ptr LoggingRegistry::formatterForName(const std::string& name) const
{
    std::lock_guard<std::mutex> lock(_mutex);

    FormatterMap::const_iterator it = _formatterMap.find(name);
    if (it != _formatterMap.end())
        return it->second;
    else
        throw PH::Exception("Not Found Exception logging formatter"+name);
}


void LoggingRegistry::registerChannel(const std::string& name, Channel::Ptr pChannel)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _channelMap[name] = pChannel;
}


void LoggingRegistry::registerFormatter(const std::string& name, Formatter::Ptr pFormatter)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _formatterMap[name] = pFormatter;
}


void LoggingRegistry::unregisterChannel(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);

    ChannelMap::iterator it = _channelMap.find(name);
    if (it != _channelMap.end())
        _channelMap.erase(it);
    else
        throw PH::Exception("Not Found Exception logging channel"+ name);
}


void LoggingRegistry::unregisterFormatter(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);

    FormatterMap::iterator it = _formatterMap.find(name);
    if (it != _formatterMap.end())
        _formatterMap.erase(it);
    else
        throw PH::Exception("Not Found Exception logging formatter"+name);
}


void LoggingRegistry::clear()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _channelMap.clear();
    _formatterMap.clear();
}

namespace
{
    static LoggingRegistry sh;
}


LoggingRegistry& LoggingRegistry::defaultRegistry()
{
    return sh;
}
}
