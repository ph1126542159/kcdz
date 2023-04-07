#ifndef ASYNCCHANNEL_H
#define ASYNCCHANNEL_H

#include "Channel.h"
#include "Message.h"
#include <mutex>
#include "Core/AutoPtr.h"
#include <QThread>
#include <QQueue>
namespace PH {
class AsyncChannel : public QThread,public Channel
{
    Q_OBJECT
public:
    using Ptr=AutoPtr<AsyncChannel>;
    explicit AsyncChannel(Channel::Ptr pChannel,QObject *parent = nullptr);
    ~AsyncChannel();
    void setChannel(Channel::Ptr ptr);
    Channel::Ptr getChannel()const;

    void open();

    void close();

    void setProperty(const std::string& name, const std::string& );

    void log(const Message &msg);

protected:
    void run();
private:
    class QueueItem{
    public:
        typedef std::shared_ptr<QueueItem> Ptr;
        QueueItem(const Message& msg):_msg(msg){

        }
        static Ptr makeItem(const Message& msg){
            Ptr ptr=std::make_shared<QueueItem>(msg);
            return ptr;
        }
        Message& msg(){
            return _msg;
        }
    private:
        Message _msg;
    };
    Channel::Ptr _pChannel;
    std::mutex _threadLock;
    std::mutex _channeLock;
    std::mutex _queueLock;
    QQueue<QueueItem::Ptr> _queue;
    int _queueSize = 0;
    int _dropCount = 0;
    std::atomic_bool _stop;
};
}


#endif // ASYNCCHANNEL_H
