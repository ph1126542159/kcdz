#ifndef ACTIVEDISPATCHER_H
#define ACTIVEDISPATCHER_H
#include <QRunnable>
#include <QThread>
#include "ActiveStarter.h"
#include "Notification/NotificationQueue.h"


namespace PH {
class MethodNotification: public Notification{
public:
    typedef PH::AutoPtr<MethodNotification> Ptr;
    MethodNotification(ActiveRunnableBase::Ptr pRunnable);

    ActiveRunnableBase::Ptr runnable()const;
private:
    ActiveRunnableBase::Ptr _pRunnable;
};
class ActiveDispatcher : public QObject,protected QRunnable
{
public:
    ActiveDispatcher();

    ActiveDispatcher(QThread::Priority prio);

    virtual ~ActiveDispatcher();

    void start(ActiveRunnableBase::Ptr pRunnable);

    void cancle();

protected:
    void run();

    void stop();

private:
    QThread _thread;
    PH::NotificationQueue _queue;
};


template<>
class ActiveStarter<ActiveDispatcher>{
public:
    static void start(ActiveDispatcher* pOwner,ActiveRunnableBase::Ptr ptr){
        pOwner->start(ptr);
    }
};
}




#endif // ACTIVEDISPATCHER_H
