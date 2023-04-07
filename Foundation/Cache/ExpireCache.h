#ifndef FUNCTION_EXPIRECACHE_INCLUDED
#define FUNCTION_EXPIRECACHE_INCLUDED
#include "AbstractCache.h"
#include "ExpireStrategy.h"

namespace PH {


template <class TKey,class TValue>
class ExpireCache : public AbstractCache<TKey,TValue,ExpireStrategy<TKey,TValue>>
{
public:

    explicit ExpireCache(uint64_t expire=500):
        AbstractCache<TKey,TValue,ExpireStrategy<TKey,TValue>>(ExpireStrategy<TKey,TValue>(expire))
    {

    }

    ~ExpireCache()
    {

    }
private:
    ExpireCache(const ExpireCache& other)=delete;
    ExpireCache& operator=(const ExpireCache& other)=delete;
};

}

#endif // FUNCTION_EXPIRECACHE_INCLUDED
