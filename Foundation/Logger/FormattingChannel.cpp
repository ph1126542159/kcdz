#include "FormattingChannel.h"
#include "Message.h"
#include "LoggingRegistry.h"
namespace PH {
FormattingChannel::FormattingChannel():
    _pFormatter(nullptr),
    _pChannel(nullptr)
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter):
    _pFormatter(pFormatter),
    _pChannel(nullptr)
{
}


FormattingChannel::FormattingChannel(Formatter::Ptr pFormatter, Channel::Ptr pChannel):
    _pFormatter(pFormatter),
    _pChannel(pChannel)
{
}

FormattingChannel::~FormattingChannel()
{
}


void FormattingChannel::setFormatter(Formatter::Ptr pFormatter)
{
    _pFormatter = pFormatter;
}


Formatter::Ptr FormattingChannel::getFormatter() const
{
    return _pFormatter;
}


void FormattingChannel::setChannel(Channel::Ptr pChannel)
{
    _pChannel = pChannel;
}


Channel::Ptr FormattingChannel::getChannel() const
{
    return _pChannel;
}


void FormattingChannel::log(const Message& msg)
{
    if (_pChannel.get())
    {
        if (_pFormatter.get())
        {
            std::string text;
            _pFormatter->format(msg, text);
            _pChannel->log(Message(msg, text));
        }
        else
        {
            _pChannel->log(msg);
        }
    }
}


void FormattingChannel::setProperty(const std::string& name, const std::string& value)
{
    if (name == "channel")
        setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
    else if (name == "formatter")
        setFormatter(LoggingRegistry::defaultRegistry().formatterForName(value));
    else if (_pChannel)
        _pChannel->setProperty(name, value);
}


void FormattingChannel::open()
{
    if (_pChannel.get())
        _pChannel->open();
}


void FormattingChannel::close()
{
    if (_pChannel.get())
        _pChannel->close();
}
}
