#include "ActiveDispatcher.h"
#include "Notification/NotificationQueue.h"

namespace PH {

MethodNotification::MethodNotification(ActiveRunnableBase::Ptr pRunnable):
    _pRunnable(pRunnable){
}
ActiveRunnableBase::Ptr MethodNotification::runnable()const{
    return _pRunnable;
}
class StopNotification: public PH::Notification{
public:
    using Ptr=std::shared_ptr<StopNotification>;
};

ActiveDispatcher::ActiveDispatcher()
{
    this->moveToThread(&_thread);
    QObject::connect(&_thread,&QThread::started,this,&ActiveDispatcher::run);
    _thread.start();
}

ActiveDispatcher::ActiveDispatcher(QThread::Priority prio)
{
    _thread.setPriority(prio);
    this->moveToThread(&_thread);
    QObject::connect(&_thread,&QThread::started,this,&ActiveDispatcher::run);
    _thread.start();
}

ActiveDispatcher::~ActiveDispatcher()
{
    try{
        stop();
    }catch(...){

    }
}

void ActiveDispatcher::start(ActiveRunnableBase::Ptr pRunnable)
{
    assert(nullptr!=pRunnable.get());
    _queue.enqueueNotification(new MethodNotification(pRunnable));
}

void ActiveDispatcher::cancle()
{
    _queue.clear();
}

void ActiveDispatcher::run()
{
    PH::Notification::Ptr pNf=_queue.waitDequeueNotification();
    while (nullptr!=pNf.get()) {
        MethodNotification::Ptr pMethodNf=pNf.cast<MethodNotification>();
        assert(nullptr!=pNf);
        ActiveRunnableBase::Ptr pRunnable=pMethodNf->runnable();
        pRunnable->run();
        pNf=_queue.waitDequeueNotification();
    }
}

void ActiveDispatcher::stop()
{
    _queue.clear();
    _queue.wakeupAll();
    _queue.enqueueNotification(new StopNotification);
    _thread.wait();
}




}
