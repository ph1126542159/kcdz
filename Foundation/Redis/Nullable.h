#ifndef NULLABLE_H
#define NULLABLE_H
#include <algorithm>
#include <iostream>
#include "RedisBugCheck.h"

namespace Redis {

enum NullType{
    NULL_GENERIC=0
};

template<typename C>
class Nullable{
public:
    Nullable():
        _value(),_isNull(true),_null(){

    }


    Nullable(const NullType&):
        _value(),_isNull(true),_null(){

    }

    Nullable(const C& value):_value(value),_isNull(false),_null(){

    }

    Nullable(C&& value):_value(std::forward<C>(value)),_isNull(false),_null(){

    }

    Nullable(const Nullable& other):_value(other._value),_isNull(other._isNull),_null(){

    }

    Nullable(Nullable&& other)noexcept:
        _value(std::move(other._value)),_isNull(other._isNull),_null(){
        other._isNull=true;
    }

    ~Nullable(){

    }

    Nullable& assign(const C& value)
    {
        _value  = value;
        _isNull = false;
        return *this;
    }

    Nullable& assign(C&& value)
    {
        _value  = std::move(value);
        _isNull = false;
        return *this;
    }

    Nullable& assign(const Nullable& other)
    {
        Nullable tmp(other);
        swap(tmp);
        return *this;
    }

    Nullable& assign(NullType)
    {
        _isNull = true;
        return *this;
    }

    Nullable& operator = (const C& value)
    {
        return assign(value);
    }

    Nullable& operator = (C&& value)
    {
        return assign(std::move(value));
    }

    Nullable& operator = (const Nullable& other)
    {
        return assign(other);
    }

    Nullable& operator = (Nullable&& other) noexcept
    {
        _isNull = other._isNull;
        _value = std::move(other._value);
        other._isNull = true;
        return *this;
    }

    Nullable& operator = (NullType)
    {
        _isNull = true;
        return *this;
    }

    void swap(Nullable& other)
    {
        std::swap(_value, other._value);
        std::swap(_isNull, other._isNull);
    }

    bool operator == (const Nullable<C>& other) const
    {
        return (_isNull && other._isNull) || (_isNull == other._isNull && _value == other._value);
    }

    bool operator == (const C& value) const
    {
        return (!_isNull && _value == value);
    }

    bool operator == (const NullType&) const
    {
        return _isNull;
    }

    bool operator != (const C& value) const
    {
        return !(*this == value);
    }

    bool operator != (const Nullable<C>& other) const
    {
        return !(*this == other);
    }

    bool operator != (const NullType&) const
    {
        return !_isNull;
    }

    bool operator < (const Nullable<C>& other) const
    {
        if (_isNull && other._isNull) return false;

        if (!_isNull && !other._isNull)
            return (_value < other._value);

        if (_isNull && !other._isNull) return true;

        return false;
    }

    bool operator > (const Nullable<C>& other) const
    {
        return !(*this == other) && !(*this < other);
    }

    C& value()
    {
        if (!_isNull)
            return _value;
        else
            throw Exception("nullptr value Exception");
    }

    const C& value() const
    {
        if (!_isNull)
            return _value;
        else
            throw Exception("nullptr value Exception");
    }

    const C& value(const C& deflt) const
    {
        return _isNull ? deflt : _value;
    }

    operator C& ()
    {
        return value();
    }

    operator const C& () const
    {
        return value();
    }

    operator NullType& ()
    {
        return _null;
    }

    bool isNull() const
    {
        return _isNull;
    }

    void clear()
    {
        _isNull = true;
    }

private:
    C _value;
    bool _isNull;
    NullType _null;
};
template <typename C>
inline void swap(Nullable<C>& n1, Nullable<C>& n2)
{
    n1.swap(n2);
}


template <typename C>
std::ostream& operator<<(std::ostream& out, const Nullable<C>& obj)
{
    if (!obj.isNull()) out << obj.value();
    return out;
}


template <typename C>
bool operator == (const NullType&, const Nullable<C>& n)
{
    return n.isNull();
}


template <typename C>
bool operator != (const C& c, const Nullable<C>& n)
{
    return !(n == c);
}


template <typename C>
bool operator == (const C& c, const Nullable<C>& n)
{
    return (n == c);
}


template <typename C>
bool operator != (const NullType&, const Nullable<C>& n)
{
    return !n.isNull();
}
}
#endif // NULLABLE_H
