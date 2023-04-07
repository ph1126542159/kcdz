#ifndef FUNCTION_ABSTRACTCACHE_INCLUDED
#define FUNCTION_ABSTRACTCACHE_INCLUDED
#include "KeyValueArgs.h"
#include "ValidArgs.h"
#include "Core/Exception.h"
#include "Events/FIFOEvent.h"
#include "Events/Delegate.h"
#include "Events/EventArgs.h"
#include "Core/SharedPtr.h"
#include <map>
#include <set>
#include <cstddef>

namespace PH {

template <class TKey,class TValue,class TStrategy>
class AbstractCache
{
public:
    AbstractCache()
    {
        initialize();
    }

    AbstractCache(const TStrategy& strat):_strategy(strat)
    {
        initialize();
    }

    virtual ~AbstractCache()
    {
        try {
            uninitialize();
        } catch (...) {
        }
    }

    void add(const TKey& key,const TValue& val)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doAdd(key,val);
    }

    void update(const TKey& key,const TValue& val)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doUpdate(key,val);
    }

    void add(const TKey& key,SharedPtr<TValue> val)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doAdd(key,val);
    }

    void update(const TKey& key,SharedPtr<TValue> val)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doUpdate(key,val);
    }

    void remove(const TKey& key)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        Iterator it=_data.find(key);
        doRemove(it);
    }

    bool has(const TKey& key) const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return doHas(key);
    }

    SharedPtr<TValue> get(const TKey& key)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return doGet(key);
    }

    void clear()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doClear();
    }

    bool isEmpty()const{
        std::unique_lock<std::mutex> lock(_mutex);
        return _data.empty();
    }

    std::size_t size()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doReplace();
        return _data.size();
    }

    void forceReplace()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doReplace();
    }

    std::set<TKey> getAllKeys()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        doReplace();

        ConstIterator it=_data.begin();
        ConstIterator itEnd=_data.end();
        std::set<TKey> result;
        for(;it!=itEnd;++it)
        {
            result.insert(it->first);
        }
        return result;
    }
protected:
    FIFOEvent<const KeyValueArgs<TKey,TValue>> Add;
    FIFOEvent<const KeyValueArgs<TKey,TValue>> Update;
    FIFOEvent<const TKey>                      Remove;
    FIFOEvent<const TKey>                      Get;
    FIFOEvent<const EventArgs>                 Clear;

    typedef std::map<TKey,SharedPtr<TValue>> DataHolder;
    typedef typename DataHolder::iterator Iterator;
    typedef typename DataHolder::const_iterator ConstIterator;
    typedef std::set<TKey>                      KeySet;

    mutable FIFOEvent<ValidArgs<TKey>> IsValid;
    mutable FIFOEvent<KeySet>          Replace;

    void initialize()
    {
        Add     +=Delegate<TStrategy,const KeyValueArgs<TKey,TValue>>(&_strategy,&TStrategy::onAdd);
        Update  +=Delegate<TStrategy,const KeyValueArgs<TKey,TValue>>(&_strategy,&TStrategy::onUpdate);
        Remove  +=Delegate<TStrategy,const TKey>(&_strategy,&TStrategy::onRemove);
        Get     +=Delegate<TStrategy,const TKey>(&_strategy,&TStrategy::onGet);
        Clear   +=Delegate<TStrategy,const EventArgs>(&_strategy,&TStrategy::onClear);
        IsValid +=Delegate<TStrategy,ValidArgs<TKey>>(&_strategy,&TStrategy::onIsValid);
        Replace +=Delegate<TStrategy,KeySet>(&_strategy,&TStrategy::onReplace);
    }

    void uninitialize()
    {
        Add     -=Delegate<TStrategy,const KeyValueArgs<TKey,TValue>>(&_strategy,&TStrategy::onAdd);
        Update  -=Delegate<TStrategy,const KeyValueArgs<TKey,TValue>>(&_strategy,&TStrategy::onUpdate);
        Remove  -=Delegate<TStrategy,const TKey>(&_strategy,&TStrategy::onRemove);
        Get     -=Delegate<TStrategy,const TKey>(&_strategy,&TStrategy::onGet);
        Clear   -=Delegate<TStrategy,const EventArgs>(&_strategy,&TStrategy::onClear);
        IsValid -=Delegate<TStrategy,ValidArgs<TKey>>(&_strategy,&TStrategy::onIsValid);
        Replace -=Delegate<TStrategy,KeySet>(&_strategy,&TStrategy::onReplace);
    }

    void doAdd(const TKey& key,const TValue& val)
    {
        Iterator it=_data.find(key);
        doRemove(it);

        KeyValueArgs<TKey,TValue> args(key,val);
        Add.notify(this,args);
        _data.insert(std::make_pair(key,SharedPtr<TValue>(new TValue(val))));

        doReplace();
    }

    void doAdd(const TKey& key,SharedPtr<TValue>& val)
    {
        Iterator it=_data.find(key);
        doRemove(it);

        KeyValueArgs<TKey,TValue> args(key,*val);
        Add.notify(this,args);
        _data.insert(std::make_pair(key,val));

        doReplace();
    }

    void doUpdate(const TKey& key,const TValue& val)
    {
        KeyValueArgs<TKey,TValue> args(key,val);
        Iterator it=_data.find(key);
        if(it==_data.end())
        {
            Add.notify(this,args);
            _data.insert(std::make_pair(key,SharedPtr<TValue>(new TValue(val))));
        }else{
            Update.notify(this,args);
            it->second=SharedPtr<TValue>(new TValue(val));
        }

        doReplace();
    }

    void doUpdate(const TKey& key,SharedPtr<TValue>& val)
    {
        KeyValueArgs<TKey,TValue> args(key,*val);
        Iterator it=_data.find(key);
        if(it==_data.end()){
            Add.notify(this,args);
            _data.insert(std::make_pair(key,val));
        }else{
            Update.notify(this,args);
            it->second=val;
        }
        doReplace();
    }

    void doRemove(Iterator it)
    {
        if(it==_data.end()) return;

        Remove.notify(this,it->first);
        _data.erase(it);
    }

    bool doHas(const TKey& key)const
    {
        ConstIterator it=_data.find(key);
        if(it==_data.end()) return false;
        ValidArgs<TKey> args(key);
        IsValid.notify(this,args);
        return args.isValid();
    }

    SharedPtr<TValue> doGet(const TKey& key)
    {
        Iterator it=_data.find(key);
        if(it==_data.end()) return nullptr;

        SharedPtr<TValue> result;
        Get.notify(this,key);
        ValidArgs<TKey> args(key);
        IsValid.notify(this,args);
        if(!args.isValid()){
            doRemove(it);
        }else{
            result=it->second;
        }
        return result;
    }

    void doClear()
    {
        static EventArgs _emptyArgs;
        Clear.notify(this,_emptyArgs);
        _data.clear();
    }

    void doReplace()
    {
        std::set<TKey> delMe;
        Replace.notify(this,delMe);

        typename std::set<TKey>::const_iterator it=delMe.begin();
        typename std::set<TKey>::const_iterator endIt=delMe.end();

        for(;it!=endIt;++it)
        {
            Iterator itH=_data.find(*it);
            doRemove(itH);
        }
    }

    TStrategy _strategy;
    mutable DataHolder _data;
    mutable std::mutex _mutex;
private:
    AbstractCache(const AbstractCache&)=delete;
    AbstractCache& operator=(const AbstractCache&)=delete;
};


}


#endif // FUNCTION_ABSTRACTCACHE_INCLUDED
