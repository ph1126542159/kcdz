#ifndef FUNCTION_DELEGATE_INCLUDED
#define FUNCTION_DELEGATE_INCLUDED
#include "AbstractDelegate.h"
#include "FunctionDelegate.h"
#include "ExpireDelegate.h"

namespace PH {

template<class TObj,class TArgs,bool withSender=true>
class Delegate : public AbstractDelegate<TArgs>
{
public:
    typedef void (TObj::*NotifyMethod)(const void*,TArgs&);

    Delegate(TObj* pObj,NotifyMethod method):
        _recvObj(pObj),_recvMethod(method)
    {

    }

    Delegate(const Delegate& other):
        AbstractDelegate<TArgs>(other),
        _recvObj(other._recvObj),
        _recvMethod(other._recvMethod)
    {

    }

    ~Delegate()
    {

    }

    Delegate& operator=(const Delegate& other)
    {
        if(&other!=this){
            this->_recvObj=other._recvObj;
            this->_recvMethod=other._recvMethod;
        }

        return *this;
    }

    bool notify(const void* sender,TArgs& args)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_recvObj&&_recvMethod){
            (_recvObj->*_recvMethod)(sender,args);
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<TArgs>& other)const
    {
        const Delegate* pDele=dynamic_cast<const Delegate*>(other.unwarp());
        return pDele&&pDele->_recvMethod==this->_recvMethod&&
                pDele->_recvObj==this->_recvObj;
    }

    AbstractDelegate<TArgs>* clone()const
    {
        return new Delegate(*this);
    }
    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _recvObj=nullptr;
    }
protected:
    TObj* _recvObj;
    NotifyMethod _recvMethod;
    mutable std::mutex _mutex;
private:
    Delegate()=delete;
};

template <class TObj,class TArgs>
class Delegate<TObj,TArgs,false>:public AbstractDelegate<TArgs>
{
public:
    typedef void (TObj::*NotifyMethod)(TArgs&);

    Delegate(TObj* pObj,NotifyMethod method):
        _recvObj(pObj),_recvMethod(method)
    {

    }

    Delegate(const Delegate& other):
        AbstractDelegate<TArgs>(other),
        _recvMethod(other._recvMethod),
        _recvObj(other._recvObj)
    {

    }

    ~Delegate()
    {

    }

    Delegate& operator=(const Delegate& other)
    {
        if(&other!=this){
            this->_recvMethod=other._recvMethod;
            this->_recvObj=other._recvObj;
        }

        return *this;
    }

    bool notify(const void*,TArgs& args)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_recvObj){
            (_recvObj->*_recvMethod)(args);
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<TArgs>& other)const
    {
        const Delegate* pDele=dynamic_cast<const Delegate*>(other.unwarp());
        return pDele&&pDele->_recvMethod==this->_recvMethod&&
                pDele->_recvObj==this->_recvObj;
    }

    AbstractDelegate<TArgs>* clone()const
    {
        return new Delegate(*this);
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _recvObj=nullptr;
    }
protected:
    TObj* _recvObj;
    NotifyMethod _recvMethod;
    mutable std::mutex _mutex;
private:
    Delegate()=delete;
};

template<class TObj,class TArgs>
inline Delegate<TObj,TArgs,true> delegate(TObj* pObj,void (TObj::*NotifyMethod)(const void*,TArgs&))
{
    return Delegate<TObj,TArgs,true>(pObj,NotifyMethod);
}

template<class TObj,class TArgs>
inline Delegate<TObj,TArgs,false> delegate(TObj* pObj,void (TObj::*NotifyMethod)(TArgs&))
{
    return Delegate<TObj,TArgs,false>(pObj,NotifyMethod);
}

template<class TObj,class TArgs>
inline ExpireDelegate<TArgs> delegate(TObj* pObj,void (TObj::*NotifyMethod)(const void*,TArgs&),uint64_t expireMillsecs)
{
    return ExpireDelegate<TArgs>(Delegate<TObj,TArgs,true>(pObj,NotifyMethod),expireMillsecs);
}

template<class TObj,class TArgs>
inline ExpireDelegate<TArgs> delegate(TObj* pObj,void (TObj::*NotifyMethod)(TArgs&),uint64_t expireMillsecs)
{
    return ExpireDelegate<TArgs>(Delegate<TObj,TArgs,false>(pObj,NotifyMethod,false),expireMillsecs);
}

template<class TArgs>
inline ExpireDelegate<TArgs> delegate(void (*NotifyMethod)(const void*,TArgs&),uint64_t expireMillsecs)
{
    return ExpireDelegate<TArgs>(FunctionDelegate<TArgs,true,true>(NotifyMethod),expireMillsecs);
}

template<class TArgs>
inline ExpireDelegate<TArgs> delegate(void (*NotifyMethod)(void* ,TArgs&),uint64_t expireMillsecs)
{
    return ExpireDelegate<TArgs>(FunctionDelegate<TArgs,true,false>(NotifyMethod),expireMillsecs);
}

template <class TArgs>
inline ExpireDelegate<TArgs> delegate(void (*NotifyMethod)(TArgs&),uint64_t expireMillsecs)
{
    return ExpireDelegate<TArgs>(FunctionDelegate<TArgs,false>(NotifyMethod),expireMillsecs);
}

template<class TArgs>
inline FunctionDelegate<TArgs,true,true> delegate(void (*NotifyMethod)(const void*,TArgs&))
{
    return FunctionDelegate<TArgs,true,true>(NotifyMethod);
}

template <class TArgs>
inline FunctionDelegate<TArgs,true,false> delegate(void (*NotifyMethod)(void*,TArgs&))
{
    return FunctionDelegate<TArgs,true,false>(NotifyMethod);
}

template <class TArgs>
inline FunctionDelegate<TArgs,false> delegate(void (*NotifyMethod)(TArgs&))
{
    return FunctionDelegate<TArgs,false>(NotifyMethod);
}

template <class TObj>
class Delegate<TObj,void,true> : public AbstractDelegate<void>
{
public:
    typedef void (TObj::*NotifyMethod)(const void*);

    Delegate(TObj* pObj,NotifyMethod method):
        _pObj(pObj),_method(method)
    {

    }

    Delegate(const Delegate& other):
        AbstractDelegate<void>(other),
        _pObj(other._pObj),_method(other._method)
    {

    }

    Delegate& operator=(const Delegate& other)
    {
        if(&other!=this){
            this->_pObj=other._pObj;
            this->_method=other._method;
        }

        return *this;
    }

    bool notify(const void* sender)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_pObj){
            (_pObj->*_method)(sender);
            return true;
        }
        return false;
    }

    bool equals(const AbstractDelegate<void>& other)const
    {
        const Delegate* pDele=dynamic_cast<const Delegate*>(other.unwarp());
        return pDele&&this->_pObj==pDele->_pObj&&this->_method==pDele->_method;
    }

    AbstractDelegate<void>* clone()const
    {
        return new Delegate<TObj,void,true>(*this);
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _pObj=nullptr;
    }

protected:
    TObj* _pObj;
    NotifyMethod _method;
    mutable std::mutex _mutex;
private:
    Delegate()=delete;
};


template <class TObj>
class Delegate<TObj,void,false> : public AbstractDelegate<void>
{
public:
  typedef void (TObj::*NotifyMethod)(void*);

    Delegate(TObj* pObj,NotifyMethod method):
        _pObj(pObj),_method(method)
    {

    }

    Delegate(const Delegate& other):
        AbstractDelegate<void>(other),
        _pObj(other._pObj),
        _method(other._method)
    {

    }

    Delegate& operator=(const Delegate& other)
    {
        if(&other!=this){
            this->_pObj=other._pObj;
            this->_method=other._method;
        }
        return *this;
    }

    bool notify(const void* sender){
        std::unique_lock<std::mutex> lock(_mutex);
        if(_pObj){
            (_pObj->*_method)(const_cast<void*>(sender));
            return true;
        }
        return false;
    }

    bool equals(const AbstractDelegate<void>& other)const
    {
        const Delegate* pDele=dynamic_cast<const Delegate*>(other.unwarp());
        return pDele&&this->_pObj==pDele->_pObj&&_method==pDele->_method;
    }

    AbstractDelegate<void>* clone()const
    {
        return new Delegate<TObj,void,false>(*this);
    }
    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _pObj=nullptr;
    }
protected:
    TObj* _pObj;
    NotifyMethod _method;
    mutable std::mutex _mutex;
private:
    Delegate()=delete;
};

template<class TObj>
inline Delegate<TObj,void,true> delegate(TObj* pObj,void (TObj::*NotifyMethod)(const void*))
{
    return Delegate<TObj,void,true>(pObj,NotifyMethod);
}

template<class TObj>
inline Delegate<TObj,void,false> delegate(TObj* pObj,void (TObj::*NotifyMethod)())
{
    return Delegate<TObj,void,false>(pObj,NotifyMethod);
}

template<class TObj>
inline ExpireDelegate<void> delegate(TObj* pObj,void (TObj::*NotifyMethod)(const void*),uint64_t expireMillsecs){
    return ExpireDelegate<void>(Delegate<TObj,void,true>(pObj,NotifyMethod),expireMillsecs);
}

template<class TObj>
inline ExpireDelegate<void> delegate(TObj* pObj,void (TObj::*NotifyMethod)(),uint64_t expireMillisecs)
{
    return ExpireDelegate<void>(Delegate<TObj,void,false>(pObj,NotifyMethod),expireMillisecs);
}

inline ExpireDelegate<void> delegate(void (*NotifyMethod)(const void*),uint64_t expireMillsecs)
{
    return ExpireDelegate<void>(FunctionDelegate<void,true,true>(NotifyMethod),expireMillsecs);
}

inline ExpireDelegate<void> delegate(void (*NotifyMethod)(void*),uint64_t expireMillsecs)
{
    return ExpireDelegate<void>(FunctionDelegate<void,true,false>(NotifyMethod),expireMillsecs);
}

inline ExpireDelegate<void> delegate(void (*NotifyMethod)(),uint64_t expireMillsecs)
{
    return ExpireDelegate<void>(FunctionDelegate<void,false>(NotifyMethod),expireMillsecs);
}

inline FunctionDelegate<void,true,true> delegate(void (*NotifyMethod)(const void*))
{
    return FunctionDelegate<void,true,true>(NotifyMethod);
}

inline FunctionDelegate<void,true,false> delegate(void (*NotifyMethod)(void*))
{
    return FunctionDelegate<void,true,false>(NotifyMethod);
}

inline FunctionDelegate<void,false> delegate(void (*NotifyMethod)())
{
    return FunctionDelegate<void,false>(NotifyMethod);
}

}


#endif // FUNCTION_DELEGATE_INCLUDED
