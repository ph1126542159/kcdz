#ifndef FOUNDATION_ABSTRACTEVENT_INCLUDED
#define FOUNDATION_ABSTRACTEVENT_INCLUDED
#include "Core/SharedPtr.h"
#include "Activetor/ActiveMethod.h"
#include <mutex>

namespace PH {

template <class TArgs,class TStrategy,class TDelegate>
class AbstractEvent{
public:
    typedef TDelegate* DelegateHandle;
    typedef TArgs Args;

    AbstractEvent():
        _executeAsync(this,&AbstractEvent::executeAsyncImpl),
        _enabled(true)
    {

    }

    AbstractEvent(const TStrategy& strat):
        _executeAsync(this,&AbstractEvent::executeAsyncImpl),
        _strategy(strat),
        _enabled(true)
    {

    }

    ~AbstractEvent()
    {

    }

    void operator +=(const TDelegate& deleg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.add(deleg);
    }

    void operator -=(const TDelegate& aDeleg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.remove(aDeleg);
    }

    DelegateHandle add(const TDelegate& deleg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _strategy.add(deleg);
    }

    void remove(DelegateHandle handle)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.remove(handle);
    }

    void operator()(const void* pSender,TArgs& args)
    {
        notify(pSender,args);
    }

    void operator()(TArgs& args)
    {
        notify(nullptr,args);
    }

    void notify(const void* pSender,TArgs& args)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(!_enabled) return;

        TStrategy strategy(_strategy);
        lock.unlock();

        strategy.notify(pSender,args);
    }

    bool hasDelegates()const
    {
        return !empty();
    }

    ActiveResult<TArgs> notifyAsync(const void* pSender,const TArgs& args)
    {
        NotifyAsyncParams params(pSender,args);
        {
            std::unique_lock<std::mutex> lock(_mutex);
            params.ptrStrat=SharedPtr<TStrategy>(new TStrategy(_strategy));
            params.enabled=_enabled;
        }

        ActiveResult<TArgs> result=_executeAsync(params);
        return result;
    }

    void enable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _enabled=true;
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _enabled=false;
    }

    bool isEnabled()const
    {
       std::unique_lock<std::mutex> lock(_mutex);
       return _enabled;
    }

    void clear()
    {
       std::unique_lock<std::mutex> lock(_mutex);
       _strategy.clear();
    }

    bool empty()const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _strategy.empty();
    }
protected:
    struct NotifyAsyncParams
    {
        SharedPtr<TStrategy> ptrStrat;
        const void* pSender;
        TArgs args;
        bool enabled;

        NotifyAsyncParams(const void* pSend,const TArgs& a):
            ptrStrat(),pSender(pSend),args(a),enabled(true){

        }
    };

    ActiveMethod<TArgs,NotifyAsyncParams,AbstractEvent> _executeAsync;

    TArgs executeAsyncImpl(const NotifyAsyncParams& par)
    {
        if(!par.enabled) return par.args;

        NotifyAsyncParams params=par;
        TArgs retArgs(params.args);
        params.ptrStrat->notify(params.pSender,retArgs);
        return retArgs;
    }

    TStrategy _strategy;
    bool _enabled;
    mutable std::mutex _mutex;
private:
    AbstractEvent(const AbstractEvent&)=delete;
    AbstractEvent& operator=(const AbstractEvent&)=delete;
};


template<class TStrategy,class TDelegate>
class AbstractEvent<void,TStrategy,TDelegate>{
public:
    typedef TDelegate* DelegateHandle;

    AbstractEvent():
        _executeAsync(this,&AbstractEvent::executeAsyncImpl),
        _enabled(true)
    {

    }

    AbstractEvent(const TStrategy& strat):
        _executeAsync(this,&AbstractEvent::executeAsyncImpl),
        _strategy(strat),
        _enabled(true)
    {

    }

    virtual ~AbstractEvent()
    {

    }

    void operator +=(const TDelegate& aDeleg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.add(aDeleg);
    }

    void operator -=(const TDelegate& aDeleg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.remove(aDeleg);
    }

    DelegateHandle add(const TDelegate& aDeleg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.add(aDeleg);
    }

    void remove(DelegateHandle handle)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.remove(handle);
    }

    void operator()(const void *pSender)
    {
        notify(pSender);
    }

    void operator()()
    {
        notify(nullptr);
    }

    void notify(const void* pSender)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if(!_enabled) return;

        TStrategy strategy(_strategy);
        lock.unlock();
        strategy.notify(pSender);
    }

    ActiveResult<void> notifyAsync(const void* pSender)
    {
        NotifyAsyncParams params(pSender);
        {
            std::unique_lock<std::mutex> lock(_mutex);
            params.ptrStrat=SharedPtr<TStrategy>(new TStrategy(_strategy));
            params.enabled=_enabled;
        }

        ActiveResult<void> result=_executeAsync(params);
        return result;
    }

    void enable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _enabled=true;
    }

    void disable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _enabled=false;
    }

    bool isEnable()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _enabled;
    }

    void clear()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _strategy.clear();
    }

    bool empty()const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _strategy.empty();
    }
protected:
    struct NotifyAsyncParams
    {
        SharedPtr<TStrategy> ptrStrat;
        const void* pSender;
        bool enabled;
        NotifyAsyncParams(const void* pSend):ptrStrat(),pSender(pSend),enabled(true) {}
    };

    ActiveMethod<void,NotifyAsyncParams,AbstractEvent> _executeAsync;

    void executeAsyncImpl(const NotifyAsyncParams& par)
    {
        if(!par.enabled) return;
        NotifyAsyncParams params=par;
        params.ptrStrat->notify(params.pSender);
        return;
    }

    TStrategy _strategy;
    bool _enabled;
    mutable std::mutex _mutex;
private:
    AbstractEvent(const AbstractEvent&)=delete;
    AbstractEvent& operator=(const AbstractEvent&)=delete;
};
}



#endif // FOUNDATION_ABSTRACTEVENT_INCLUDED
