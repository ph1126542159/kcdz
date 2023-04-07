#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H
#include <string>
#include "Core/RefCountedObject.h"
namespace PH {
class   Configurable : public RefCountedObject
{
public:
    Configurable();
    std::string getName()const;
    virtual ~Configurable();

    virtual void setProperty(const std::string& name, const std::string& value) = 0;

    virtual std::string getProperty(const std::string& name) const = 0;
};
}

#endif // CONFIGURABLE_H
