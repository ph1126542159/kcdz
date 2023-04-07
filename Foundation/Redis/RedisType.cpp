#include "RedisType.h"
#include "RedisError.h"
#include "RedisArray.h"
namespace Redis {
RedisType::RedisType()
{
}


RedisType::~RedisType()
{
}


RedisType::Ptr RedisType::createRedisType(char marker)
{
    RedisType::Ptr result;

    switch(marker)
    {
    case RedisTypeTraits<qint64>::marker :
        result.reset(new Type<qint64>());
        break;
    case RedisTypeTraits<std::string>::marker :
        result.reset(new Type<std::string>());
        break;
    case RedisTypeTraits<BulkString>::marker :
        result.reset(new Type<BulkString>());
        break;
    case RedisTypeTraits<Array>::marker :
        result.reset(new Type<Array>());
        break;
    case RedisTypeTraits<Error>::marker :
        result.reset(new Type<Error>());
        break;
    }
    return result;
}
}
