#include "RedisError.h"

namespace Redis {
Error::Error()
{
}


Error::Error(const std::string& message): _message(message)
{
}


Error::~Error()
{
}
}
