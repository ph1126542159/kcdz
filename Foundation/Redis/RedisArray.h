#ifndef REDISARRAY_H
#define REDISARRAY_H
#include <vector>
#include <sstream>
#include "RedisType.h"
#include "RedisBugCheck.h"
namespace Redis {
class Array{
public:
    using const_iterator = std::vector<RedisType::Ptr>::const_iterator;

    Array();
    Array(const Array& copy);

    virtual ~Array();

    template<typename T>
    Array& operator<<(const T& arg)
    {
        return add(arg);
    }

    Array& operator<<(const char* s);

    Array& operator<<(const std::vector<std::string>& strings);

    Array& add();

    template<typename T>
    Array& add(const T& arg)
    {
        std::shared_ptr<Type<T>> ptr;
        ptr.reset(new Type<T>(arg));
        addRedisType(std::static_pointer_cast<RedisType>(ptr));
        return *this;
    }

    Array& add(const char* s);

    Array& add(const std::vector<std::string>& strings);

    Array& addRedisType(RedisType::Ptr value);

    Array& addSimpleString(const std::string& value);

    const_iterator begin() const;

    void clear();

    const_iterator end() const;

    template<typename T>
    T get(size_t pos) const
    {
        if ( _elements.isNull() ) throw NullValueException();

        if ( pos >= _elements.value().size() ) throw InvalidArgumentException();

        RedisType::Ptr element = _elements.value().at(pos);
        if ( RedisTypeTraits<T>::TypeId == element->type() )
        {
            Type<T>* concrete = dynamic_cast<Type<T>* >(element.get());
            if ( concrete != NULL ) return concrete->value();
        }
        throw Exception("Array get Bad Cast Exception");
    }

    int getType(size_t pos) const;

    bool isNull() const;

    void makeNull();

    std::string toString() const;

    size_t size() const;
private:
    void checkNull();
    Nullable<std::vector<RedisType::Ptr>> _elements;
};

inline Array& Array::operator<<(const char* s)
{
    BulkString value(s);
    return add(value);
}


inline Array& Array::operator<<(const std::vector<std::string>& strings)
{
    return add(strings);
}


inline Array& Array::add()
{
    BulkString value;
    return add(value);
}


template<>
inline Array& Array::add(const std::string& arg)
{
    BulkString value(arg);
    return add(value);
}


inline Array& Array::add(const char* s)
{
    BulkString value(s);
    return add(value);
}


inline Array& Array::add(const std::vector<std::string>& strings)
{
    for(std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); ++it)
    {
        add(*it);
    }
    return *this;
}


inline Array& Array::addSimpleString(const std::string& value)
{
    std::shared_ptr<Type<std::string>> ptr;
    ptr.reset(new Type<std::string>(value));
    return addRedisType(std::static_pointer_cast<RedisType>(ptr));
}


inline Array::const_iterator Array::begin() const
{
    return _elements.value().begin();
}


inline void Array::checkNull()
{
    std::vector<RedisType::Ptr> v;
    if ( _elements.isNull() ) _elements.assign(v);
}


inline void Array::clear()
{
    if ( !_elements.isNull() )
    {
        _elements.value().clear();
    }
}


inline Array::const_iterator Array::end() const
{
    return _elements.value().end();
}


inline bool Array::isNull() const
{
    return _elements.isNull();
}


inline void Array::makeNull()
{
    if ( !_elements.isNull() ) _elements.value().clear();

    _elements.clear();
}


inline size_t Array::size() const
{
    return _elements.value().size();
}

template<>
struct RedisTypeTraits<Array>
{
    enum { TypeId = RedisType::REDIS_ARRAY };

    static const char marker = '*';

    static std::string toString(const Array& value)
    {
        std::stringstream result;
        result <<  marker;
        if ( value.isNull() )
        {
            result << "-1" << "\r\n";
        }
        else
        {
            result << value.size() << "\r\n";
            for(std::vector<RedisType::Ptr>::const_iterator it = value.begin();
                it != value.end(); ++it)
            {
                result << (*it)->toString();
            }
        }
        return std::string(result.str().data());
    }

    static void read(RedisInputStream& input, Array& value)
    {
        value.clear();

        qint64 length = QString(input.getline().c_str()).toLongLong();

        if ( length != -1 )
        {
            for(int i = 0; i < length; ++i)
            {
                char marker = input.get();
                RedisType::Ptr element = RedisType::createRedisType(marker);
                if (nullptr==element.get())
                    throw Exception("Wrong answer received from Redis server");
                element->read(input);
                value.addRedisType(element);
            }
        }
    }
};

}

#endif // REDISARRAY_H


