#ifndef FUNCTION_FIFOEVENT_INCLUDED
#define FUNCTION_FIFOEVENT_INCLUDED
#include "AbstractEvent.h"
#include "FIFOStrategy.h"
#include "AbstractDelegate.h"

namespace  PH {

template <class TArgs>
class FIFOEvent : public AbstractEvent<TArgs,
        FIFOStrategy<TArgs,AbstractDelegate<TArgs>>,
        AbstractDelegate<TArgs>>
{
public:
    FIFOEvent()
    {

    }

    ~FIFOEvent()
    {

    }

private:
    FIFOEvent(const FIFOEvent& e)=delete;
    FIFOEvent& operator=(const FIFOEvent& e)=delete;
};

}

#endif // FUNCTION_FIFOEVENT_INCLUDED
