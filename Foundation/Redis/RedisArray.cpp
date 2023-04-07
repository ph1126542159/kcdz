#include "RedisArray.h"

namespace Redis{
Array::Array()
{
}


Array::Array(const Array& copy):
    _elements(copy._elements)
{
}


Array::~Array()
{
}


Array& Array::addRedisType(RedisType::Ptr value)
{
    checkNull();

    _elements.value().push_back(value);

    return *this;
}


int Array::getType(size_t pos) const
{
    if (_elements.isNull()) throw Exception("nullptr value Exception");

    if (pos >= _elements.value().size()) throw Exception("Invalid Argument Exception");

    RedisType::Ptr element = _elements.value().at(pos);
    return element->type();
}


std::string Array::toString() const
{
    return RedisTypeTraits<Array>::toString(*this);
}
}
