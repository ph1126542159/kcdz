#ifndef FUNCTION_EXPIRESTRATEGY_INCLUDED
#define FUNCTION_EXPIRESTRATEGY_INCLUDED
#include "KeyValueArgs.h"
#include "ValidArgs.h"
#include "AbstractStrategy.h"
#include <set>
#include <map>
#include <QElapsedTimer>

namespace  PH {
///缓存 过期 机制
template <class TKey,class TValue>
class ExpireStrategy : public AbstractStrategy<TKey,TValue>
{
public:
    typedef std::multimap<QElapsedTimer,TKey> TimeIndex;
    typedef typename TimeIndex::iterator IndexIterator;
    typedef typename TimeIndex::const_iterator ConstIndexIterator;
    typedef std::map<TKey,IndexIterator> Keys;
    typedef typename Keys::iterator Iterator;

    ExpireStrategy(uint64_t expireTm/*毫秒级*/):
        _expireTime(expireTm)
    {
        if(_expireTime <25) throw InvalidArgumentException("expiretime must be at least 25ms");
    }

    ~ExpireStrategy()
    {

    }

    void onAdd(const void*,const KeyValueArgs<TKey,TValue>& args)
    {
        QElapsedTimer now;
        now.start();
        typename TimeIndex::value_type tiValue(now,args.key());
        IndexIterator it=_keyIndex.insert(tiValue);
        typename Keys::value_type kValue(args.key(),it);
        std::pair<Iterator,bool> stat=_keys.insert(kValue);
        if(!stat.second)
        {
            _keyIndex.erase(stat.first->second);
            stat.first->second=it;
        }
    }

    void onRemove(const void*,const TKey& key)
    {
        Iterator it=_keys.find(key);
        if(it!=_keys.end()){
            _keyIndex.erase(it->second);
            _keys.erase(it);
        }
    }

    void onGet(const void*,const TKey& )
    {

    }

    void onClear(const void*,const EventArgs&)
    {
        _keys.clear();
        _keyIndex.clear();
    }

    void onIsValid(const void*,ValidArgs<TKey>& args)
    {
        Iterator it=_keys.find(args.key());
        if(it==_keys.end()){
            args.invalidate();
            return;
        }
        if(it->second->first.hasExpired(_expireTime)){
            return args.invalidate();
        }
    }

    void onReplace(const void*,std::set<TKey>& elems)
    {
        IndexIterator it=_keyIndex.begin();
        while (it!=_keyIndex.end()&&it->first.hasExpired(_expireTime)) {
            elems.insert(it->second);
            ++it;
        }
    }

protected:
    uint64_t _expireTime;
    Keys _keys;
    TimeIndex _keyIndex;
};

}

#endif // FUNCTION_EXPIRESTRATEGY_INCLUDED
