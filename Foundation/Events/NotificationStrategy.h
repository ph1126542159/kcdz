#ifndef FOUNDATION_NOTIFICATIONSTRATEGY_INCLUDED
#define FOUNDATION_NOTIFICATIONSTRATEGY_INCLUDED
namespace PH {
template<class TArgs,class TDelegate>
class NotificationStrtegy
{
public:
    typedef TDelegate* DelegateHandle;

    NotificationStrtegy()
    {

    }

    virtual ~NotificationStrtegy()
    {

    }

    virtual void notify(const void* sender,TArgs& args)=0;

    virtual DelegateHandle add(const TDelegate& deleg)=0;

    virtual void remove(const TDelegate& delegate)=0;

    virtual void remove(DelegateHandle deleHandle)=0;

    virtual void clear()=0;

    virtual bool empty()const=0;
};

template<class TDelegate>
class NotificationStrtegy<void,TDelegate>
{
  public:
    typedef TDelegate* DelegateHandle;

    NotificationStrtegy()
    {

    }

    virtual ~NotificationStrtegy()
    {

    }

    virtual void notify(const void* sender)=0;

    virtual DelegateHandle add(const TDelegate& deletegate)=0;

    virtual void remove(const TDelegate& delegate)=0;

    virtual void remove(DelegateHandle handle)=0;

    virtual void clear()=0;

    virtual bool empty()const=0;
};

}
#endif // FOUNDATION_NOTIFICATIONSTRATEGY_INCLUDED
