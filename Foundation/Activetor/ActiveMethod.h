#ifndef ACTIVEMETHOD_H
#define ACTIVEMETHOD_H
#include "ActiveResult.h"
#include "ActiveRunnable.h"
#include "ActiveStarter.h"
#include <QDebug>


namespace PH {


template<class ResultType,class ArgType,class OwnerType,class StarterType=ActiveStarter<OwnerType>>
class ActiveMethod{
public:
    typedef ResultType (OwnerType::*Callback)(const ArgType&);
    typedef ActiveResult<ResultType> ActiveResultType;
    typedef ActiveRunnable<ResultType,ArgType,OwnerType> ActiveRunnableType;

    ActiveMethod(OwnerType* pOwner,Callback method):
        _pOwner(pOwner),_method(method){
        assert(nullptr!=_pOwner);
    }

    ActiveResultType operator()(const ArgType& arg){
        ActiveResultType result(new ActiveResultHolder<ResultType>());
        ActiveRunnableType* pRunnable=new ActiveRunnableType(_pOwner, _method,arg,result);
        StarterType::start(_pOwner,pRunnable);
        return result;
    }

    ActiveMethod(const ActiveMethod& other):
        _pOwner(other._pOwner),
        _method(other._method){

    }

    ActiveMethod& operator =(const ActiveMethod& other){
        ActiveMethod tmp(other);
        swap(tmp);
        return *this;
    }

    void swap(ActiveMethod other){
        std::swap(_pOwner,other._pOwner);
        std::swap(_method,other._method);
    }

private:
    ActiveMethod()=delete;

    OwnerType* _pOwner;
    Callback _method;
};

template<class ResultType,class OwnerType,class StarterType>
class ActiveMethod<ResultType,void,OwnerType,StarterType>{
public:
    typedef ResultType (OwnerType::*Callback)(void);
    typedef ActiveResult<ResultType> ActiveResultType;
    typedef ActiveRunnable<ResultType,void,OwnerType> ActiveRunnableType;

    ActiveMethod(OwnerType* pOwner,Callback method):
        _pOwner(pOwner),_method(method){
        assert(nullptr!=pOwner);
    }

    ActiveResultType operator ()(void){
        ActiveResultType result(new ActiveResultHolder<ResultType>());
        ActiveRunnableBase::Ptr pRunnable=std::make_shared<ActiveRunnableType>(_pOwner,_method,result);
        StarterType::start(_pOwner,pRunnable);
        return result;
    }

    ActiveMethod(const ActiveMethod& other):
        _pOwner(other._pOwner),
        _method(other._method){

    }

    ActiveMethod& operator=(const ActiveMethod& other){
        ActiveMethod tmp(other);
        swap(tmp);
        return *this;
    }

    void swap(const ActiveMethod& other){
        std::swap(_pOwner,other._pOwner);
        std::swap(_method,other._method);
    }

private:
    ActiveMethod()=delete;

    OwnerType* _pOwner;
    Callback _method;
};
}

#endif // ACTIVEMETHOD_H
