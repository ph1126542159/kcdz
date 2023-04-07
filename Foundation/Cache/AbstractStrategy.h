#ifndef FUNCTION_ABSTRACTSTRATEGY_INCLUDED
#define FUNCTION_ABSTRACTSTRATEGY_INCLUDED

#include "KeyValueArgs.h"
#include "ValidArgs.h"
#include "Events/EventArgs.h"
#include <set>

namespace  PH {
template <class TKey,class TValue>
class AbstractStrategy
{
public:
    AbstractStrategy()
    {

    }

    virtual ~AbstractStrategy()
    {

    }

    virtual void onUpdate(const void* pSender,const KeyValueArgs<TKey,TValue>& args)
    {
        onRemove(pSender,args.key());
        onAdd(pSender,args);
    }

    virtual void onAdd(const void* pSender,const KeyValueArgs<TKey,TValue>& key)=0;

    virtual void onGet(const void * pSender,const TKey& key)=0;

    virtual void onRemove(const void* pSender,const TKey& key)=0;

    virtual void onClear(const void* pSender,const EventArgs& args)=0;

    virtual void onIsValid(const void *pSender,ValidArgs<TKey>& key)=0;

    virtual void onReplace(const void* pSender,std::set<TKey>& elems)=0;
};

}

#endif // FUNCTION_ABSTRACTSTRATEGY_INCLUDED
