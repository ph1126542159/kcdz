#ifndef REDISCLIENT_H
#define REDISCLIENT_H
#include "RedisArray.h"
#include "RedisStream.h"
#include "RedisError.h"
#include "RedisBugCheck.h"
#include <QHostAddress>
#include <QDebug>
namespace Redis {
class RedisClient
{
public:
    using Ptr = std::shared_ptr<RedisClient>;
    RedisClient();

    virtual ~RedisClient();

    QHostAddress address() const;

    bool connect(const std::string& host, unsigned short port,int msecs = 30000);

    bool connect(const QHostAddress& addrs, unsigned short port,int msecs = 30000);

    void disconnect();

    bool isConnected() const;

    template<typename T>
    T execute(const Array& command){
        T result = T();
        writeCommand(command, true);
        readReply(result);
        return result;
    }

    void flush();

    RedisType::Ptr sendCommand(const Array& command);

    RedisType::Ptr readReply();

    template<typename T>
    void readReply(T& result){
        RedisType::Ptr redisResult = readReply();
        if (redisResult->type() == RedisTypeTraits<Error>::TypeId)
        {
            Type<Error>* error = dynamic_cast<Type<Error>*>(redisResult.get());
            throw Exception(error->value().getMessage());
        }

        if (redisResult->type() == RedisTypeTraits<T>::TypeId)
        {
            Type<T>* type = dynamic_cast<Type<T>*>(redisResult.get());
            if (type != NULL) result = type->value();
        }
        else throw Exception("Bad Cast Exception");
    }

    Array sendCommands(const std::vector<Array>& commands);

    QTcpSocket* getSocket(){return &_socket;}
private:
    RedisClient(const RedisClient&)=delete;
    RedisClient& operator = (const RedisClient&)=delete;

    bool connect(int msecs = 30000);

    void writeCommand(const Array& command, bool flush);

    unsigned short _port;
    QHostAddress _address;
    QTcpSocket _socket;
    RedisInputStream* _input;
    RedisOutputStream* _output;
    bool _isConnected;
};


inline QHostAddress RedisClient::address() const
{
    return _address;
}


template<>
inline void RedisClient::execute<void>(const Array& command)
{
    writeCommand(command, false);
}


inline void RedisClient::flush()
{
    assert(nullptr!=_output);
    _output->flush();
}

}



#endif // REDISCLIENT_H
