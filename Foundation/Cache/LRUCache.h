#ifndef FUNCTION_LRUCACHE_INCLUDED
#define FUNCTION_LRUCACHE_INCLUDED
#include "AbstractCache.h"
#include "LRUStrategy.h"

namespace  PH {
template <class TKey,class TValue>
class LRUCache:public AbstractCache<TKey,TValue,LRUStrategy<TKey,TValue>>
{
public:

    explicit LRUCache(long nSize=1024):
        AbstractCache<TKey,TValue,LRUStrategy<TKey,TValue>>(LRUStrategy<TKey,TValue>(nSize))
    {

    }

    ~LRUCache()
    {

    }
private:
    LRUCache(const LRUCache&)=delete;
    LRUCache& operator=(const LRUCache& other)=delete;
};
}

#endif // FUNCTION_LRUCACHE_INCLUDED
