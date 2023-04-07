#ifndef FOUNDATION_EXPIREDELEGATE_INCLUDED
#define FOUNDATION_EXPIREDELEGATE_INCLUDED
#include "AbstractDelegate.h"
#include <QElapsedTimer>

namespace PH {

template <class TArgs>
class ExpireDelegate : public AbstractDelegate<TArgs>
{
public:

    ExpireDelegate(const AbstractDelegate<TArgs>& p,uint64_t experiMillisecs):
        _pDelegate(p.clone()),
        _expire(experiMillisecs)
    {
        _createTm.start();
    }

    ExpireDelegate(const ExpireDelegate& other):
        AbstractDelegate<TArgs>(other),
        _pDelegate(other._pDelegate->clone()),
        _expire(other._expire),
        _createTm(other._createTm)
    {
    }

    ~ExpireDelegate(){
        delete _pDelegate;
    }

    ExpireDelegate& operator=(const ExpireDelegate& other)
    {
        if(&other!=this)
        {
            delete this->_pDelegate;
            this->_pDelegate=other._pDelegate->clone();
            this->_expire=other._expire;
            this->_createTm=other._createTm;
        }

        return *this;
    }

    bool notify(const void* sender,TArgs& args)
    {
        if(!expired()) return this->_pDelegate->notify(sender,args);
        return false;
    }

    bool equals(const AbstractDelegate<TArgs>& other)const
    {
        return other.equals(*_pDelegate);
    }

    AbstractDelegate<TArgs>* clone()const
    {
        return new ExpireDelegate(*this);
    }

    void disable()
    {
        return _pDelegate->disable();
    }
    const AbstractDelegate<TArgs>* unwarp()const
    {
        return this->_pDelegate;
    }
protected:
    bool expired()const
    {
        ///微秒级
        return quint64(_createTm.elapsed())>_expire;
    }

    AbstractDelegate<TArgs> *_pDelegate;
    uint64_t _expire;
    QElapsedTimer _createTm;
private:
    ExpireDelegate()=delete;
};

template<>
class ExpireDelegate<void>:public AbstractDelegate<void>
{
public:
    ExpireDelegate(const AbstractDelegate<void>& op,uint64_t experiMillisecs):
        _pDelegate(op.clone()),_expire(experiMillisecs){
        _createTm.start();
    }

    ExpireDelegate(const ExpireDelegate& expire):
        AbstractDelegate<void>(expire),
        _pDelegate(expire._pDelegate->clone()),
        _expire(expire._expire),
        _createTm(expire._createTm)
    {
    }

    ~ExpireDelegate()
    {
        delete _pDelegate;
    }

    ExpireDelegate& operator =(const ExpireDelegate& expire)
    {
        if(&expire!=this)
        {
            delete this->_pDelegate;
            this->_pDelegate=expire._pDelegate->clone();
            this->_expire=expire._expire;
            this->_createTm=expire._createTm;
        }

        return *this;
    }

    bool notify(const void* sender)
    {
        if(!expired()) return this->_pDelegate->notify(sender);

        return false;
    }

    bool equals(const AbstractDelegate<void>& other)const
    {
        return other.equals(*_pDelegate);
    }

    AbstractDelegate<void>* clone()const
    {
        return new ExpireDelegate(*this);
    }

    void disable()
    {
        _pDelegate->disable();
    }
    const AbstractDelegate<void>* unwarp()const
    {
        return this->_pDelegate;
    }
protected:
    bool expired()const
    {
        return quint64(_createTm.elapsed())>_expire;
    }
    AbstractDelegate<void>* _pDelegate;
    uint64_t _expire;
    QElapsedTimer _createTm;
private:
    ExpireDelegate()=delete;
};


}
#endif // FOUNDATION_EXPIREDELEGATE_INCLUDED
