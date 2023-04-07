#ifndef REDISERROR_H
#define REDISERROR_H
#include "RedisType.h"
namespace Redis {
class  Error
{
public:
    Error();

    Error(const std::string& message);

    virtual ~Error();

    const std::string& getMessage() const;

    void setMessage(const std::string& message);

private:
    std::string _message;
};



inline const std::string& Error::getMessage() const
{
    return _message;
}


inline void Error::setMessage(const std::string& message)
{
    _message = message;
}


template<>
struct RedisTypeTraits<Error>
{
    enum { TypeId = RedisType::REDIS_ERROR };

    static const char marker = '-';

    static std::string toString(const Error& value)
    {
        return marker + value.getMessage()  + "\r\n";
    }

    static void read(RedisInputStream& input, Error& value)
    {
        value.setMessage(input.getline().c_str());
    }
};
}

#endif // REDISERROR_H
