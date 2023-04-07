#include "AsyncChannel.h"
#include "LoggingRegistry.h"
#include <iostream>
namespace PH {
AsyncChannel::AsyncChannel(Channel::Ptr pChannel, QObject *parent):QThread(parent)
{
        _pChannel=pChannel;
        _stop=false;
        this->setObjectName("AsyncChannel");
}

AsyncChannel::~AsyncChannel()
{
    try{
        close();
    }catch(...){

    }
}

void AsyncChannel::setChannel(Channel::Ptr ptr)
{
    std::unique_lock<std::mutex> lock(_channeLock);
    _pChannel=ptr;
}

Channel::Ptr AsyncChannel::getChannel() const
{
    return _pChannel;
}

void AsyncChannel::open()
{
    if(this->isRunning()) return;
    _pChannel->open();
    std::unique_lock<std::mutex> lock(_threadLock);
    this->start();
}

void AsyncChannel::close()
{
    if(!this->isRunning()) return;
    _stop=true;
    {
        std::unique_lock<std::mutex> lock(_queueLock);
        _queue.clear();
    }
    this->quit();
    while (!this->wait(100)) ;
}

void AsyncChannel::setProperty(const std::string& name, const std::string& value)
{
    if("channel"==name){
        setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
        return;
    }

    if("queueSize"==name){
        if(value.compare("none")==0||value.compare("unlimited")||value.empty()) _queueSize=0;
        else  _queueSize=std::stoi(value);
        return;
    }

    Channel::setProperty(name,value);
}

void AsyncChannel::log(const Message &msg)
{
    if (_queueSize != 0 && _queue.size() >= _queueSize)
    {
        ++_dropCount;
        return;
    }
    if (_dropCount != 0)
    {
        {
            std::unique_lock<std::mutex> lock(_queueLock);
            Message dropMsg(msg,QString("Dropped %1 messages.").arg(_dropCount).toStdString());
            _queue.append(QueueItem::makeItem(dropMsg));

            _dropCount = 0;
        }
    }

    open();
    {
        std::unique_lock<std::mutex> lock(_queueLock);
        _queue.append(QueueItem::makeItem(msg));
    }
}

void AsyncChannel::run()
{
    while (!_stop) {
        if(nullptr!=_pChannel.get())
        {
            std::unique_lock<std::mutex> lock(_queueLock);
            while (!_queue.isEmpty()) {
                QueueItem::Ptr ptr= _queue.front();
                _queue.pop_front();
                _pChannel->log(ptr->msg());
            }
        }
        QThread::msleep(10);
    }
}

}

