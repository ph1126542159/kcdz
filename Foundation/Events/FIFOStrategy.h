#ifndef FOUNCTION_FIFOSTRATEGY_INCLUDED
#define FOUNCTION_FIFOSTRATEGY_INCLUDED

#include "DefaultStrategy.h"

namespace PH {
template <class TArgs,class TDelegate>
class FIFOStrategy : public DefaultStrategy<TArgs,TDelegate>
{
  public:
    FIFOStrategy()
    {

    }

    FIFOStrategy(const FIFOStrategy& s):
        DefaultStrategy<TArgs,TDelegate>(s)
    {

    }

    ~FIFOStrategy()
    {

    }

    FIFOStrategy& operator =(const FIFOStrategy& s)
    {
        DefaultStrategy<TArgs,TDelegate>::operator=(s);
        return *this;
    }
};

}


#endif // FOUNCTION_FIFOSTRATEGY_INCLUDED
