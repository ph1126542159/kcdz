#ifndef FORMATTINGCHANNEL_H
#define FORMATTINGCHANNEL_H
#include "Channel.h"
#include "Formatter.h"


namespace PH {


class Formatter;


class   FormattingChannel: public Channel
{
public:
    using Ptr = AutoPtr<FormattingChannel>;

    FormattingChannel();

    FormattingChannel(Formatter::Ptr pFormatter);

    FormattingChannel(Formatter::Ptr pFormatter, Channel::Ptr pChannel);

    void setFormatter(Formatter::Ptr pFormatter);

    Formatter::Ptr getFormatter() const;

    void setChannel(Channel::Ptr pChannel);

    Channel::Ptr getChannel() const;

    void log(const Message& msg);

    void setProperty(const std::string& name, const std::string& value);

    void open();

    void close();

    ~FormattingChannel();

private:
    Formatter::Ptr _pFormatter;
    Channel::Ptr   _pChannel;
};


}

#endif // FORMATTINGCHANNEL_H
