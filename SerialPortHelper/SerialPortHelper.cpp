#include "SerialPortHelper.h"
#include <atomic>
#include "Core/SharedPtr.h"
#include "SerialPortHandler.h"
#include <QElapsedTimer>
#include <QThreadPool>
#include <QSerialPortInfo>
#include <QDebug>
namespace kcdz {
class SerialPortHelper::Private{
public:
    PH::SharedPtr<QSerialPort> _pSerial;
    PH::SharedPtr<Handler> _handler;
    std::atomic_bool _stop;
    QString _comName;
    QSerialPort::BaudRate _baud;
    QSerialPort::DataBits _dbits;
    QSerialPort::Parity _par;
    QSerialPort::StopBits _sBits;
    QSerialPort::FlowControl _fc;
    QElapsedTimer _tm;
    QStringList _serialList;
    Private():_stop(false){
    }
    void open(){
        if(_pSerial.isNull()){
                    _pSerial=new QSerialPort;
        }
        _pSerial->setPortName(_comName);
        _pSerial->setBaudRate(_baud);
        _pSerial->setDataBits(_dbits);
        _pSerial->setParity(_par);
        _pSerial->setStopBits(_sBits);
        _pSerial->setFlowControl(_fc);
        while (!_stop&&_pSerial->isOpen()) {
            QThread::msleep(1000);
            qDebug()<<"isConnectting SerialPort,COM name:"<<_comName<<",Baud:"<<_baud;
        }
    }
    void close(){
        _pSerial->close();
    }
};
void SerialPortHelper::stop()
{
    p->_stop=true;
    while (!p->_pSerial.isNull()&&p->_pSerial->isOpen()) {
        QThread::msleep(0);
    }
    if(!p->_handler.isNull()){
            p->_handler->clearRecvBuffer();
            p->_handler->clearSendBuffer();
    }
}

void SerialPortHelper::start()
{
    if(!p->_stop) return;
    QThreadPool::globalInstance()->start(this);
    p->_stop=false;
}

void SerialPortHelper::restart()
{
    stop();
    start();
}

SerialPortHelper::SerialPortHelper(QObject *parent) : QObject(parent)
{
    p=new Private;
    p->_stop=false;
}

SerialPortHelper::~SerialPortHelper()
{
    stop();
    delete p;
    p=nullptr;
}
QString SerialPortHelper::byteArrayToHexString(QByteArray ascii)
{
    QString ret;
    for(int i = 0; i < ascii.count(); i++)
        ret.append(QString("%1 ").arg((uchar)ascii.at(i), 2, 16, (QChar)'0'));
    return ret.toUpper();
}
QString SerialPortHelper::byteArrToString(const char *data, int nLen)
{
    int nCount=0;
    QString sstr;
    while (nCount<nLen) {
        if('\0'==data[nCount]){
            sstr+='0';
        }else{
            QChar ch(data[nCount]);
            if(ch.isDigit()||ch.isLetter()){
                sstr+=data[nCount];
            }else if(ch.isNumber()){
                sstr+=QString::number(data[nCount]);
            }
        }
        ++nCount;
    }
    return sstr;
}
QByteArray SerialPortHelper::hexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(s.isEmpty())  continue;
        char c = s.toInt(&ok,16)&0xFF;
        if(!ok) continue;
        ret.append(c);
    }
    return ret;
}
QString SerialPortHelper::comName(){
    return p->_comName;
}
void SerialPortHelper::setComName(const QString& value){
    p->_comName=value;
}


QVariant SerialPortHelper::baudRate(){
    return p->_baud;
}
void SerialPortHelper::setBaudRate(const QVariant& value){
    p->_baud=value.value<QSerialPort::BaudRate>();
}


QVariant SerialPortHelper::dataBits(){
    return p->_dbits;
}
void SerialPortHelper::setDataBits(const QVariant& value)
{
    p->_dbits=value.value<QSerialPort::DataBits>();
}


QVariant  SerialPortHelper::parity(){
    return p->_par;
}
void SerialPortHelper::setParity(const QVariant & value){
    p->_par=value.value<QSerialPort::Parity>();
}


QVariant SerialPortHelper::stopBits(){
    return  p->_sBits;
}
void SerialPortHelper::setStopBits(const QVariant& value){
    p->_sBits=value.value<QSerialPort::StopBits>();
}


QVariant SerialPortHelper::flowControl(){
    return p->_fc;
}
void SerialPortHelper::setFlowControl(const QVariant& value){
    p->_fc=value.value<QSerialPort::FlowControl>();
}
void SerialPortHelper::setHanler(Handler * value){
    p->_handler=value;
}
Handler *SerialPortHelper::hanler() const{
    return p->_handler.get();
}
void SerialPortHelper::run()
{
    p->open();
    emit serialPortConnected();
    while (!p->_stop) {
        ///send
        if(!p->_handler.isNull()){
            QByteArray bt=p->_handler->getSend();
            if(bt.size()>0){
                p->_pSerial->write(bt.data(),bt.size());
                p->_pSerial->flush();
            }
        }
        ///read
        if(p->_pSerial->waitForReadyRead(50)){
            if(!p->_handler.isNull()){
                p->_handler->appendRecv(p->_pSerial->readAll());
                p->_handler->parseData();
            }
        }
        if(p->_tm.elapsed()>1000){
            p->_tm.restart();
            QStringList listStr;
            foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts()){
                listStr<<Info.portName();
            }
            if(listStr!=p->_serialList){
                p->_serialList=listStr;
                emit serialPortListChanged(listStr);
            }
        }

    }
    p->close();
    emit serialPortDisConnected();
}
}
