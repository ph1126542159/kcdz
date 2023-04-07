#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <mutex>
namespace kcdz {
class Handler : public QObject
{
    Q_OBJECT
public:
    class Private{
    public:
        void appendRecv(const QByteArray& arr){
            std::unique_lock<std::mutex> lock(_mutexRecv);
            _recvBuffer.append(arr.data(),arr.size());
        }
        void appendSend(const QByteArray& arr){
            std::unique_lock<std::mutex> lock(_mutexSend);
            _sendBuffer.append(arr.data(),arr.size());
        }
        void clearSendBuffer(){
            std::unique_lock<std::mutex> lock(_mutexSend);
            _sendBuffer.clear();
        }

        void clearRecvBuffer(){
            std::unique_lock<std::mutex> lock(_mutexRecv);
            _recvBuffer.clear();
        }

        QByteArray readData(int nLen){
            assert(nLen>0);
            std::unique_lock<std::mutex> lock(_mutexRecv);
            if(nLen>_recvBuffer.size()) return QByteArray();
            QByteArray bt(_recvBuffer.data(),nLen);
            _recvBuffer.remove(0,nLen);
            return bt;
        }
        QByteArray getSend(){
            std::unique_lock<std::mutex> lock(_mutexSend);
            QByteArray bt(_sendBuffer.data(),_sendBuffer.size());
            _sendBuffer.clear();
            return bt;
        }
        QByteArray _recvBuffer;
        QByteArray _sendBuffer;
        mutable std::mutex _mutexSend;
        mutable std::mutex _mutexRecv;
    };
    explicit Handler(QObject *parent = nullptr);
    virtual ~Handler();
    void appendRecv(const QByteArray& arr);
    void appendSend(const QByteArray& arr);
    QByteArray readData(int nLen);
    void clearSendBuffer();
    void clearRecvBuffer();
    QByteArray getSend();
    virtual void parseData()=0;
private:
    Private* p;
};
}

#endif // SERIALPORTHANDLER_H
