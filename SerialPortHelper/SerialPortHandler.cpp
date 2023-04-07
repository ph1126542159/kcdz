#include "SerialPortHandler.h"

namespace kcdz {
Handler::Handler(QObject *parent) : QObject(parent)
{
    p=new Private;
}

Handler::~Handler()
{
    delete p;
}

void Handler::appendRecv(const QByteArray &arr)
{
    p->appendRecv(arr);
}

void Handler::appendSend(const QByteArray &arr)
{
    p->appendSend(arr);
}

QByteArray Handler::readData(int nLen)
{
    return p->readData(nLen);
}

void Handler::clearSendBuffer()
{
    p->clearSendBuffer();
}

void Handler::clearRecvBuffer()
{
    p->clearRecvBuffer();
}

QByteArray Handler::getSend()
{
    return p->getSend();
}
}

