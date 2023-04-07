#include "TcpClient.h"
#include <atomic>
#include <QTcpSocket>
#include "Core/SharedPtr.h"
#include "SerialPortHelper/SerialPortHandler.h"
#include <QThreadPool>
namespace kcdz {
class TcpClient::Private
{
public:
    Private():_stop(true),_handler(nullptr)
    {
    }
    void connect(){
        if(_tcpClient.isNull()){
            _tcpClient=new QTcpSocket;
        }
        _tcpClient->abort();
        while (!_stop&&!_tcpClient->waitForConnected(300)) {
            _tcpClient->connectToHost(_ip,_port);

        }
    }
    void disconnect(){
        if(_tcpClient.isNull()) return;
        _tcpClient->abort();
        _tcpClient->close();
    }
    QString _ip;
    qint16 _port;
    std::atomic_bool _stop;
    PH::SharedPtr<Handler> _handler;
    PH::SharedPtr<QTcpSocket> _tcpClient;
};

void TcpClient::stop()
{
    p->_stop=true;
    while (!p->_tcpClient.isNull()&&p->_tcpClient->isOpen()) {
        QThread::msleep(0);
    }
    if(!p->_handler.isNull()){
        p->_handler->clearRecvBuffer();
        p->_handler->clearSendBuffer();
    }
}

void TcpClient::start()
{
    if(!p->_stop) return;
    QThreadPool::globalInstance()->start(this);
    p->_stop=false;
}

void TcpClient::restart()
{
    stop();
    start();
}

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{
    p=new Private;
}

TcpClient::~TcpClient()
{
    stop();
    delete p;
    p=nullptr;
}
void TcpClient::setIp(const QString& value){
    p->_ip=value;
}
QString TcpClient::ip()const{
    return p->_ip;
}
qint16 TcpClient::port( ){
    return p->_port;
}
void TcpClient::setPort(qint16 value)const{
    p->_port=value;
}
void TcpClient::setHanler(Handler * value){
    p->_handler=value;
}
Handler *TcpClient::hanler() const{
    return p->_handler.get();
}
void TcpClient::run()
{
    p->connect();
    emit connected();
    while (!p->_stop) {
        ///send
        if(!p->_handler.isNull()){
            QByteArray bt=p->_handler->getSend();
            if(bt.size()>0){
                p->_tcpClient->write(bt.data(),bt.size());
                p->_tcpClient->flush();
            }
        }
        ///read
        if(p->_tcpClient->waitForReadyRead(50))
        {
            if(!p->_handler.isNull()){
                p->_handler->appendRecv(p->_tcpClient->readAll());
                p->_handler->parseData();
            }
        }
    }
    p->disconnect();
    emit disConnected();
}
}

