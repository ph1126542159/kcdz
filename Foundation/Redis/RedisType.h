#ifndef REDISTYPE_H
#define REDISTYPE_H
#include "Nullable.h"
#include "RedisStream.h"
#include <QString>
namespace Redis {
class RedisType{
public:
    enum Types
    {
        REDIS_INTEGER,       /// Redis Integer
        REDIS_SIMPLE_STRING, /// Redis Simple String
        REDIS_BULK_STRING,   /// Redis Bulkstring
        REDIS_ARRAY,         /// Redis Array
        REDIS_ERROR          /// Redis Error
    };

    using Ptr=std::shared_ptr<RedisType>;

    RedisType();

    virtual ~RedisType();

    bool isArray()const;

    bool isBulkString()const;

    bool isError()const;

    bool isInteger()const;

    bool isSimpleString()const;

    virtual int type()const=0;

    virtual void read(RedisInputStream& input)=0;

    virtual std::string toString() const=0;

    static RedisType::Ptr createRedisType(char marker);
    ///     - '+': a simple string (std::string)
    ///     - '-': an error (Error)
    ///     - '$': a bulk string (BulkString)
    ///     - '*': an array (Array)
    ///     - ':': a signed 64 bit integer (Int64)
};


inline bool RedisType::isArray() const
{
    return type() == REDIS_ARRAY;
}


inline bool RedisType::isBulkString() const
{
    return type() == REDIS_BULK_STRING;
}


inline bool RedisType::isError() const
{
    return type() == REDIS_ERROR;
}


inline bool RedisType::isInteger() const
{
    return type() == REDIS_INTEGER;
}


inline bool RedisType::isSimpleString() const
{
    return type() == REDIS_SIMPLE_STRING;
}

template<typename T>
struct RedisTypeTraits{

};

template<>
struct RedisTypeTraits<qint64>
{
    enum
    {
        TypeId = RedisType::REDIS_INTEGER
    };

    static const char marker = ':';

    static std::string toString(const qint64& value)
    {
        return marker + QString::number(value).toStdString() + "\r\n";
    }

    static void read(RedisInputStream& input, qint64& value)
    {
        std::string number = input.getline();
        value = QString(number.c_str()).toLongLong();
    }
};
template<>
struct RedisTypeTraits<std::string>
{
    enum
    {
        TypeId = RedisType::REDIS_SIMPLE_STRING
    };

    static const char marker = '+';

    static std::string toString(const std::string& value)
    {
        return marker + value  + "\r\n";
    }

    static void read(RedisInputStream& input, std::string& value)
    {
        value = input.getline();
    }
};
using BulkString = Nullable<std::string>;

template<>
struct RedisTypeTraits<BulkString>
{
    enum
    {
        TypeId = RedisType::REDIS_BULK_STRING
    };

    static const char marker = '$';

    static std::string toString(const BulkString& value)
    {
        if ( value.isNull() )
        {
            return marker + std::string("-1") + "\r\n";
        }
        else
        {
            std::string s = value.value();
            return marker
                    + QString::number(s.length()).toStdString()
                    + "\r\n"
                    + s
                    + "\r\n";
        }
    }

    static void read(RedisInputStream& input, BulkString& value)
    {
        value.clear();

        std::string line = input.getline();
        int length = QString(line.c_str()).toInt();

        if ( length >= 0 )
        {
            std::string s;
            s.resize(length, ' ');
            input.read(&*s.begin(), length);
            value.assign(s);
            input.getline(); // Read and ignore /r/n
        }
    }
};

template <typename T>
class Type : public RedisType{
public:
    Type()
    {
    }

    Type(const T& t) : _value(t)
    {
    }

    Type(const Type& copy) : _value(copy._value)
    {
    }

    virtual ~Type()
    {
    }

    int type() const
    {
        return RedisTypeTraits<T>::TypeId;
    }

    void read(RedisInputStream& socket)
    {
        RedisTypeTraits<T>::read(socket, _value);
    }

    std::string toString() const
    {
        return RedisTypeTraits<T>::toString(_value);
    }

    T& value()
    {
        return _value;
    }

    const T& value() const
    {
        return _value;
    }

private:
    T _value;
};

}

#endif // REDISTYPE_H
