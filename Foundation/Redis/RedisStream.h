#ifndef REDISSTREAM_H
#define REDISSTREAM_H
#include "BufferStreamBuf.h"
#include <istream>
#include <ostream>
#include <QTcpSocket>

namespace Redis {
class RedisStream :public BufferedStreamBuf
{
public:
    RedisStream(QTcpSocket& redis);
    ~RedisStream();
protected:
    int readFromDevice(char* buffer, std::streamsize length);
    int writeToDevice(const char* buffer, std::streamsize length);
private:
    enum
    {
        STREAM_BUFFER_SIZE = 1024
    };

    QTcpSocket& _redis;
};

class RedisIOS : public virtual std::ios{
public:
    RedisIOS(QTcpSocket& redis);
    ~RedisIOS();
    RedisStream* rdbuf();
    void close();
protected:
    RedisStream _buf;
};


class RedisOutputStream :public RedisIOS ,public std::ostream{
public:
    RedisOutputStream(QTcpSocket& redis);

    ~RedisOutputStream();
};
class  RedisInputStream: public RedisIOS, public std::istream
{
public:
    RedisInputStream(QTcpSocket& redis);

    ~RedisInputStream();

    std::string getline();
};
}

#endif // REDISSTREAM_H
