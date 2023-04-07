#ifndef FUNCTION_LRUSTRATEGY_INCLUDED
#define FUNCTION_LRUSTRATEGY_INCLUDED
#include "KeyValueArgs.h"
#include "ValidArgs.h"
#include "AbstractStrategy.h"
#include "Events/EventArgs.h"
#include "Core/Exception.h"
#include <list>
#include <map>
#include <cstddef>


namespace PH {
///缓存 固定个数 策略
template<class TKey,class TValue>
class LRUStrategy : public AbstractStrategy<TKey,TValue>
{
public:
    typedef std::list<TKey> Keys;
    typedef typename Keys::iterator Iterator;
    typedef typename Keys::const_iterator ConstIterator;
    typedef std::map<TKey,Iterator> KeyIndex;
    typedef typename KeyIndex::iterator IndexIterator;
    typedef typename KeyIndex::const_iterator ConstIndexIterator;

    LRUStrategy(std::size_t nSize)
        :_size(nSize)
    {
        if(_size<1) throw InvalidArgumentException("size must be >0");
    }

    ~LRUStrategy()
    {

    }

    void onAdd(const void*,const KeyValueArgs<TKey,TValue>& args)
    {
        _keys.push_front(args.key());
        std::pair<IndexIterator,bool> stat=_keyIndex.insert(std::make_pair(args.key(),_keys.begin()));
        if(!stat.second)
        {
            stat.first->second=_keys.begin();
        }
    }

    void onRemove(const void*,const TKey& key)
    {
        IndexIterator it=_keyIndex.find(key);
        if(it!=_keyIndex.end())
        {
            _keys.erase(it->second);
            _keyIndex.erase(it);
        }
    }

    void onGet(const void*,const TKey& key)
    {
        IndexIterator it=_keyIndex.find(key);
        if(it!=_keyIndex.end())
        {
            _keys.splice(_keys.begin(),_keys,it->second);
            it->second=_keys.begin();
        }
    }

    void onClear(const void*,const EventArgs&)
    {
        _keys.clear();
        _keyIndex.clear();
    }

    void onIsValid(const void*,ValidArgs<TKey>& args)
    {
        if(_keyIndex.find(args.key())==_keyIndex.end())
            args.invalidate();
    }

    void onReplace(const void*,std::set<TKey>& elems)
    {
        std::size_t curSize=_keyIndex.size();
        if(curSize<_size) return;

        std::size_t diff=curSize-_size;
        Iterator it=--_keys.end();
        std::size_t i=0;
        while (i++<diff) {
            elems.insert(*it);
            if(it!=_keys.begin()) --it;
        }

    }
protected:
    std::size_t _size;
    Keys _keys;
    KeyIndex _keyIndex;
};

}


#endif // FUNCTION_LRUSTRATEGY_INCLUDED
