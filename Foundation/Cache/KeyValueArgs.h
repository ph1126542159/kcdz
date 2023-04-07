#ifndef FOUNCTION_KEYVALUEARGS_INCLUDED
#define FOUNCTION_KEYVALUEARGS_INCLUDED

namespace PH {
template<class TKey,class TValue>
class KeyValueArgs{
public:
    KeyValueArgs(const TKey& aKey,const TValue& aVal):
        _key(aKey),_value(aVal)
    {

    }

    KeyValueArgs(const KeyValueArgs& other):
        _key(other._key),_value(other._value)
    {

    }

    ~KeyValueArgs()
    {

    }

    const TKey& key()const
    {
        return _key;
    }

    const TValue& value()const
    {
        return _value;
    }
protected:
    const TKey& _key;
    const TValue& _value;
private:
    KeyValueArgs& operator=(const KeyValueArgs& args)=delete;
};
}


#endif // FOUNCTION_KEYVALUEARGS_INCLUDED
