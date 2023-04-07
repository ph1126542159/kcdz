#ifndef FUNCTION_STRATEGYCOLLECTION_INCLUDED
#define FUNCTION_STRATEGYCOLLECTION_INCLUDED
#include "KeyValueArgs.h"
#include "ValidArgs.h"
#include "AbstractStrategy.h"
#include "Core/SharedPtr.h"
#include <vector>


namespace  PH {
///多种缓存策略的集合
template <class TKey,class TValue>
class StrategyCollection : public AbstractStrategy<TKey,TValue>
{
public:
    typedef std::vector<SharedPtr<AbstractStrategy<TKey,TValue>>> Strategies;
    typedef typename Strategies::iterator Iterator;
    typedef typename Strategies::const_iterator ConstIterator;

    StrategyCollection()
    {

    }

    ~StrategyCollection()
    {

    }

    void pushBack(AbstractStrategy<TKey,TValue>* pStrat)
    {
        _strategies.push_back(SharedPtr<AbstractStrategy<TKey,TValue>>(pStrat));
    }

    void popBack()
    {
        _strategies.pop_back();
    }

    void onAdd(const void* pSender,const KeyValueArgs<TKey,TValue>& key)
    {
        Iterator it=_strategies.begin();
        Iterator endIt=_strategies.end();
        for(;it!=endIt;++it)
        {
            (*it)->onAdd(pSender,key);
        }
    }

    void onRemove(const void* pSender,const TKey& key)
    {
        Iterator it=_strategies.begin();
        Iterator endIt=_strategies.end();
        for(;it!=endIt;++it)
        {
            (*it)->onRemove(pSender,key);
        }
    }

    void onGet(const void* pSender,const TKey &key)
    {
        Iterator it=_strategies.begin();
        Iterator endIt=_strategies.end();
        for(;it!=endIt;++it)
        {
            (*it)->onGet(pSender,key);
        }
    }

    void onClear(const void* pSender,const EventArgs& args)
    {
        Iterator it=_strategies.begin();
        Iterator endIt=_strategies.end();
        for(;it!=endIt;++it)
        {
            (*it)->onClear(pSender,args);
        }
    }

    void onIsValid(const void* pSender,ValidArgs<TKey>& key)
    {
        Iterator it=_strategies.begin();
        Iterator endIt=_strategies.end();
        for(;it!=endIt&&key.isValid();++it)
        {
            (*it)->onIsValid(pSender,key);
        }
    }

    void onReplace(const void* pSender,std::set<TKey>& elems)
    {
        Iterator it=_strategies.begin();
        Iterator endIt=_strategies.end();
        for(;it!=endIt;++it)
        {
            (*it)->onReplace(pSender,elems);
        }
    }

protected:
    Strategies _strategies;
};

}

#endif // FUNCTION_STRATEGYCOLLECTION_INCLUDED
