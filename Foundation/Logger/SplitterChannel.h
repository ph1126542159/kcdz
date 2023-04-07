#ifndef SPLITTERCHANNEL_H
#define SPLITTERCHANNEL_H
#include "Channel.h"
#include "Core/AutoPtr.h"
#include <vector>
#include <mutex>
#include <QString>
namespace PH {
class SplitterChannel : public Channel{
public:
    SplitterChannel();

    void addChannel(Channel::Ptr pChannel);

    void removeChannel(Channel::Ptr pChannel);

    void log(const Message& msg);

    void setProperty(const std::string& name, const std::string& value)override;

    void close();

    int count() ;

protected:
    ~SplitterChannel();

private:
    typedef std::vector<Channel::Ptr> ChannelVec;
    ChannelVec        _channels;
    std::mutex _mutex;
};
}
#endif // SPLITTERCHANNEL_H
