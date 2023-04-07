#ifndef FUNCTION_EXPIRELRUCACHE_INCLUDED
#define FUNCTION_EXPIRELRUCACHE_INCLUDED

#include "AbstractCache.h"
#include "StrategyCollection.h"
#include "ExpireStrategy.h"
#include "LRUStrategy.h"

namespace  PH {
template <class TKey,class TValue>
class ExpireLRUCache : public AbstractCache<TKey,TValue,StrategyCollection<TKey,TValue>>
{
public:

    ExpireLRUCache(long cacheSize,uint64_t expire=60*1000):
        AbstractCache<TKey,TValue,StrategyCollection<TKey,TValue>>(StrategyCollection<TKey,TValue>())
    {
        this->_strategy.pushBack(new LRUStrategy<TKey,TValue>(cacheSize));
        this->_strategy.pushBack(new ExpireStrategy<TKey,TValue>(expire));
    }

    ~ExpireLRUCache()
    {

    }
private:
    ExpireLRUCache(const ExpireLRUCache&)=delete;
    ExpireLRUCache& operator=(const ExpireLRUCache&)=delete;
};

}
#endif // FUNCTION_EXPIRELRUCACHE_INCLUDED
