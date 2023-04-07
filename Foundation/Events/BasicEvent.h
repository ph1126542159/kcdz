#ifndef FOUNDATION_BASICEVENT_INCLUDED
#define FOUNDATION_BASICEVENT_INCLUDED
#include "AbstractEvent.h"
#include "DefaultStrategy.h"
#include "AbstractDelegate.h"


namespace PH
{

template<class TArgs>
class BasicEvent : public AbstractEvent<TArgs,DefaultStrategy<TArgs,AbstractDelegate<TArgs>>,AbstractDelegate<TArgs>>
{
public:
    BasicEvent()
    {

    }

    ~BasicEvent()
    {

    }
private:
    BasicEvent(const BasicEvent& other)=delete;
    BasicEvent& operator=(const BasicEvent& other);
};


}

#endif // FOUNDATION_BASICEVENT_INCLUDED
