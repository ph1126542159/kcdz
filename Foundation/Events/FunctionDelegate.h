#ifndef FOUNDATION_FOUNDATIONDELEGATE_INCLUDED
#define FOUNDATION_FOUNDATIONDELEGATE_INCLUDED

#include "AbstractDelegate.h"
#include <mutex>
namespace PH {
template<class TArgs,bool hasSender=true,bool senderIsConst=true>
class FunctionDelegate : public AbstractDelegate<TArgs>
{
public:
    typedef void (*NotifyFunction)(const void*,TArgs&);

    FunctionDelegate(NotifyFunction func):
        _function(func){

    }

    FunctionDelegate(const FunctionDelegate& other):
        AbstractDelegate<TArgs>(other),
        _function(other._function)
    {

    }

    ~FunctionDelegate()
    {

    }

    FunctionDelegate& operator=(const FunctionDelegate& delegate)
    {
        if(&delegate!=this)
        {
            this->_function=delegate._function;
        }
        return *this;
    }

    bool notify(const void* sender,TArgs& argument)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_function){
            (*_function)(sender,argument);
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<TArgs>& other)const
    {
        const FunctionDelegate* pDele=dynamic_cast<const FunctionDelegate*>(other.unwarp());
        return pDele&&_function==pDele->_function;
    }

    AbstractDelegate<TArgs>* clone()const
    {
        return new FunctionDelegate(*this);
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _function=nullptr;
    }
protected:
    NotifyFunction _function;
    mutable std::mutex _mutex;
private:
    FunctionDelegate()=delete;
};

template<class TArgs>
class FunctionDelegate<TArgs,true,false>:public AbstractDelegate<TArgs>
{
public:
    typedef void (*NotifyFunction)(void*,TArgs&);

    FunctionDelegate(NotifyFunction func):
        _function(func){

    }

    FunctionDelegate(const FunctionDelegate& delegate):
        AbstractDelegate<TArgs>(delegate),
        _function(delegate._function){

    }

    ~FunctionDelegate()
    {

    }

    FunctionDelegate& operator=(const FunctionDelegate& delegate)
    {
        if(&delegate!=this)
        {
            this->_function=delegate._function;
        }
        return *this;
    }

    bool notify(const void* sender,TArgs& args)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_function){
            (*_function)(const_cast<void*>(sender),args);
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<TArgs>& other)const
    {
        const FunctionDelegate* pDele=dynamic_cast<const FunctionDelegate*>(other.unwarp());
        return pDele&&_function==pDele->_function;
    }

    AbstractDelegate<TArgs>* clone()const
    {
        return new FunctionDelegate(*this);
    }
    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _function=nullptr;
    }
protected:
    NotifyFunction _function;
    mutable std::mutex _mutex;
private:
    FunctionDelegate()=delete;
};

template<class TArgs>
class FunctionDelegate<TArgs,false>:public AbstractDelegate<TArgs>
{
public:
    typedef void (*NotifyFunction)(TArgs&);
    FunctionDelegate(NotifyFunction function):
        _function(function)
    {
    }

    FunctionDelegate(const FunctionDelegate& delegate):
        AbstractDelegate<TArgs>(delegate),
        _function(delegate._function)
    {
    }

    ~FunctionDelegate()
    {
    }

    FunctionDelegate& operator = (const FunctionDelegate& delegate)
    {
        if (&delegate != this)
        {
            this->_function = delegate._function;
        }
        return *this;
    }

    bool notify(const void*,TArgs& args)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_function){
            (*_function)(args);
            return true;
        }

        return false;
    }
    bool equals(const AbstractDelegate<TArgs>& other)const
    {
        const FunctionDelegate* pDele=dynamic_cast<const FunctionDelegate*>(other.unwarp());
        return pDele&&_function==pDele->_function;
    }

    AbstractDelegate<TArgs>* clone()const
    {
        return new FunctionDelegate(*this);
    }
    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _function=nullptr;
    }
protected:
    NotifyFunction _function;
    mutable std::mutex _mutex;
private:
    FunctionDelegate();
};

template<>
class FunctionDelegate<void,true,true> : public AbstractDelegate<void>
{
public:
    typedef void (*NotifyFunction)(const void*);

    FunctionDelegate(NotifyFunction func):
        _function(func)
    {

    }

    FunctionDelegate(const FunctionDelegate& other):
        AbstractDelegate<void>(other),
        _function(other._function)
    {

    }

    ~FunctionDelegate()
    {

    }

    FunctionDelegate& operator=(const FunctionDelegate& other)
    {
        if(&other!=this){
            this->_function=other._function;
        }

        return *this;
    }

    bool notify(const void* sender)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_function){
            (*_function)(sender);
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<void>& other)const
    {
        const FunctionDelegate* pDele=dynamic_cast<const FunctionDelegate*>(other.unwarp());
        return pDele&&_function==pDele->_function;
    }

    AbstractDelegate<void>* clone()const
    {
        return new FunctionDelegate(*this);
    }
    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _function=nullptr;
    }
protected:
    NotifyFunction _function;
    mutable std::mutex _mutex;
private:
    FunctionDelegate()=delete;
};

template<>
class FunctionDelegate<void,true,false> : public AbstractDelegate<void>
{
public:
    typedef void (*NotifyFunction)(void *);

    FunctionDelegate(NotifyFunction func):
        _function(func)
    {

    }

    FunctionDelegate(const FunctionDelegate& other):
       AbstractDelegate<void>(other),
       _function(other._function)
    {

    }

    ~FunctionDelegate()
    {

    }

    FunctionDelegate& operator=(const FunctionDelegate& other)
    {
        if(&other!=this){
            this->_function=other._function;
        }

        return *this;
    }

    bool notify(const void * sender)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_function){
            (*_function)(const_cast<void*>(sender));
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<void>& other)const
    {
        const FunctionDelegate* pDelete=dynamic_cast<const FunctionDelegate*>(other.unwarp());
        return pDelete&&_function==pDelete->_function;
    }

    AbstractDelegate<void>* clone()const
    {
        return new FunctionDelegate(*this);
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _function=nullptr;
    }

protected:
    NotifyFunction _function;
    mutable std::mutex _mutex;
private:
    FunctionDelegate()=delete;
};


template<>
class FunctionDelegate<void,false>:public AbstractDelegate<void>
{
public:
  typedef void (*NotifyFunction)();

    FunctionDelegate(NotifyFunction func):
        _function(func){

    }

    FunctionDelegate(const FunctionDelegate& other):
      AbstractDelegate<void>(other),
      _function(other._function)
    {

    }

    ~FunctionDelegate()
    {

    }

    FunctionDelegate& operator=(const FunctionDelegate& other)
    {
        if(&other!=this)
        {
            _function=other._function;
        }

        return *this;
    }

    bool notify(const void*)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(_function)
        {
            (*_function)();
            return true;
        }

        return false;
    }

    bool equals(const AbstractDelegate<void>& other)const
    {
        const FunctionDelegate* pDele=dynamic_cast<const FunctionDelegate*>(other.unwarp());
        return pDele&&_function==pDele->_function;
    }

    AbstractDelegate<void>* clone()const
    {
        return new FunctionDelegate(*this);
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _function=nullptr;
    }
protected:
    NotifyFunction _function;
    mutable std::mutex _mutex;
private:
    FunctionDelegate()=delete;
};
}//end namespace PH
#endif // FOUNDATION_FOUNDATIONDELEGATE_INCLUDED
