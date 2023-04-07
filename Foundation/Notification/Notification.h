﻿#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <string>
#include "Core/AutoPtr.h"
#include "Core/RefCountedObject.h"
namespace PH {
class Notification: public RefCountedObject
{
public:
    using Ptr=PH::AutoPtr<Notification>;
    Notification();

    virtual std::string name()const;
protected:
    ~Notification();
};
}


#endif // NOTIFICATION_H
