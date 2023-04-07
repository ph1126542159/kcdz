#ifndef FUNCTION_VALIDARGS_INCLUDED
#define FUNCTION_VALIDARGS_INCLUDED

namespace PH {
template <class TKey>
class ValidArgs
{
public:

    ValidArgs(const TKey& key):
        _key(key),
        _isValid(true)
    {

    }

    ValidArgs(const ValidArgs& args):
        _key(args._key),_isValid(args._isValid)
    {

    }

    ~ValidArgs()
    {

    }

    const TKey& key()const
    {
        return _key;
    }

    bool isValid()const
    {
        return _isValid;
    }

    void invalidate()
    {
        _isValid=false;
    }
protected:
    const TKey& _key;
    bool _isValid;
private:
    ValidArgs& operator=(const ValidArgs&)=delete;
};
}

#endif // FUNCTION_VALIDARGS_INCLUDED
