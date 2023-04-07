#ifndef ACTIVERUNNABLE_H
#define ACTIVERUNNABLE_H
#include "ActiveResult.h"
#include "Core/AutoPtr.h"
#include "Core/RefCountedObject.h"
#include <QRunnable>
#include <memory>
#include <QDebug>
namespace PH {

class ActiveRunnableBase : public QRunnable,public RefCountedObject{
public:
    typedef PH::AutoPtr<ActiveRunnableBase> Ptr;
};

template<class ResultType,class ArgType,class OwnerType>
class ActiveRunnable : public ActiveRunnableBase{
public:
    typedef ResultType(OwnerType::*Callback)(const ArgType&);
    typedef ActiveResult<ResultType> ActiveResultType;

    ActiveRunnable(OwnerType* pOwner,Callback method,const ArgType& arg,const ActiveResultType& result):
        _pOwner(pOwner),_method(method),_arg(arg),_result(result){
        assert(nullptr!=pOwner);
    }

    void run()override{
        try{
            _result.data(new ResultType((_pOwner->*_method)(_arg)));
        }catch(PH::Exception& e){
            _result.error(e);
        }catch(std::exception& e){
            _result.error(e.what());
        }catch(...){
            _result.error("unkonw exception");
        }
        _result.notify();
    }
private:
    OwnerType* _pOwner;
    Callback _method;
    ArgType _arg;
    ActiveResultType _result;
};


template<class ArgType,class OwnerType>
class ActiveRunnable<void,ArgType,OwnerType>:public ActiveRunnableBase{
public:
    typedef void(OwnerType::*Callback)(const ArgType& );
    typedef ActiveResult<void> ActiveResultType;

    ActiveRunnable(OwnerType* pOwner,Callback method,const ArgType& arg,const ActiveResultType& result):
        _pOwner(pOwner),_method(method),_arg(arg),_result(result){
        assert(nullptr!=pOwner);
    }

    void run()override{
        try{
            (_pOwner->*_method)(_arg);
        }catch(Exception& e){
            _result.error(e);
        }catch(std::exception& e){
            _result.error(e.what());
        }catch(...){
            _result.error("unknow error.");
        }
        _result.notify();
    }
private:
    OwnerType* _pOwner;
    Callback _method;
    ArgType _arg;
    ActiveResultType _result;
};

template<class ResultType,class OwnerType>
class ActiveRunnable<ResultType,void,OwnerType> : public ActiveRunnableBase{
  public:
    typedef ResultType(OwnerType::*Callback)(void);
    typedef ActiveResult<ResultType> ActiveResultType;

    ActiveRunnable(OwnerType* pOwner,Callback method,const ActiveResultType& result):
        _pOwner(pOwner),_method(method),_result(result){
        assert(nullptr!=pOwner);
    }

    void run()override{
        try{
            _result.data(new ResultType((_pOwner->*_method)()));
        }catch(PH::Exception& e){
            _result.error(e);
        }catch(std::exception &e){
            _result.error(e.what());
        }catch(...){
            _result.error("unknow error.");
        }

        _result.notify();
    }
private:
    OwnerType* _pOwner;
    Callback _method;
    ActiveResultType _result;
};

template<class OwnerType>
class ActiveRunnable<void,void,OwnerType>:public ActiveRunnableBase{
  public:
    typedef void (OwnerType::*Callback)(void);
    typedef ActiveResult<void> ActiveResultType;
    ActiveRunnable(OwnerType* pOwner,Callback method,const ActiveResultType& result):
        _pOwner(pOwner),
        _result(result),
        _method(method){
        assert(nullptr!=_pOwner);
    }

    void run()override{
        try{
            (_pOwner->*_method)();
        }catch(PH::Exception& exc){
            _result.error(exc);
        }catch(std::exception& e){
            _result.error(e.what());
        }catch(...){
            _result.error("unknow error.");
        }
        _result.notify();
    }
private:
    OwnerType* _pOwner;
    ActiveResultType _result;
    Callback _method;
};
}

#endif // ACTIVERUNNABLE_H
