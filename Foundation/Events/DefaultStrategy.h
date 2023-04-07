#ifndef FOUNDATION_DEFAULTSTRATEGY_INCLUDED
#define FOUNDATION_DEFAULTSTRATEGY_INCLUDED
#include "NotificationStrategy.h"
#include "Core/SharedPtr.h"
#include <vector>

namespace PH {

template<class TArgs,class TDelegate>
class DefaultStrategy : public NotificationStrtegy<TArgs,TDelegate>
{
public:
    typedef TDelegate* DelegateHandle;
    typedef SharedPtr<TDelegate> DelegatePtr;
    typedef std::vector<DelegatePtr> Delegates;
    typedef typename Delegates::iterator Iterator;

    DefaultStrategy()
    {

    }

    DefaultStrategy(const DefaultStrategy& other)
        :_delegates(other._delegates)
    {

    }

    ~DefaultStrategy()
    {

    }

    void notify(const void* sender,TArgs& args)
    {
        for(auto it:_delegates)
        {
            it->notify(sender,args);
        }
    }

    DelegateHandle add(const TDelegate& delegate)
    {
        DelegatePtr pDele(static_cast<TDelegate*>(delegate.clone()));
        _delegates.push_back(pDele);
        return pDele.get();
    }

    void remove(DelegateHandle handle)
    {
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (*it == handle)
            {
                (*it)->disable();
                _delegates.erase(it);
                return;
            }
        }
    }

    void remove(const TDelegate& delegate)
    {
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (delegate.equals(**it))
            {
                (*it)->disable();
                _delegates.erase(it);
                return;
            }
        }
    }

    DefaultStrategy& operator=(const DefaultStrategy& other)
    {
        if(this!=&other){
            _delegates=other._delegates;
        }
        return *this;
    }

    void clear()
    {
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            (*it)->disable();
        }
        _delegates.clear();
    }

    bool empty()const
    {
        return _delegates.empty();
    }

protected:
    Delegates _delegates;
};

template<class TDelegate>
class DefaultStrategy<void,TDelegate>:public NotificationStrtegy<void,TDelegate>
{
public:
    typedef TDelegate* DelegateHandle;
    typedef SharedPtr<TDelegate> DelegatePtr;
    typedef std::vector<DelegatePtr> Delegates;
    typedef typename Delegates::iterator Iterator;

    DefaultStrategy()
    {

    }

    DefaultStrategy(const DefaultStrategy& other):
        _delegates(other._delegates){

    }

    ~DefaultStrategy()
    {

    }

    void notify(const void* sender)
    {
        for(auto it:_delegates)
        {
            it->notify(sender);
        }
    }

    DelegateHandle add(const TDelegate& delegate)
    {
        DelegatePtr pDele(static_cast<TDelegate*>(delegate.clone()));
        _delegates.push_back(pDele);
        return pDele.get();
    }

    void remove(const TDelegate& delegate)
    {
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (delegate.equals(**it))
            {
                (*it)->disable();
                _delegates.erase(it);
                return;
            }
        }
    }

    void remove(DelegateHandle handle)
    {
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            if (*it == handle)
            {
                (*it)->disable();
                _delegates.erase(it);
                return;
            }
        }
    }

    DefaultStrategy& operator=(const DefaultStrategy& other)
    {
        if(this!=&other)
        {
            _delegates=other._delegates;
        }
        return *this;
    }

    void clear()
    {
        for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
        {
            (*it)->disable();
        }
        _delegates.clear();
    }

    bool empty()const
    {
        return _delegates.empty();
    }
protected:
    Delegates _delegates;
};


}


#endif // FOUNDATION_DEFAULTSTRATEGY_INCLUDED
