#ifndef LOGGINGREGISTRY_H
#define LOGGINGREGISTRY_H

#include "Channel.h"
#include "Formatter.h"
#include <mutex>
#include <map>

namespace PH {


class   LoggingRegistry
{
public:
    LoggingRegistry();

    ~LoggingRegistry();

    Channel::Ptr channelForName(const std::string& name) const;

    Formatter::Ptr formatterForName(const std::string& name) const;


    void registerChannel(const std::string& name, Channel::Ptr pChannel);


    void registerFormatter(const std::string& name, Formatter::Ptr pFormatter);


    void unregisterChannel(const std::string& name);

    void unregisterFormatter(const std::string& name);


    void clear();


    static LoggingRegistry& defaultRegistry();


private:
    typedef Channel::Ptr ChannelPtr;
    typedef AutoPtr<Formatter> FormatterPtr;
    typedef std::map<std::string, ChannelPtr> ChannelMap;
    typedef std::map<std::string, FormatterPtr> FormatterMap;

    ChannelMap   _channelMap;
    FormatterMap _formatterMap;
    mutable std::mutex _mutex;
};


}


#endif // LOGGINGREGISTRY_H
