#ifndef FORMATTER_H
#define FORMATTER_H
#include "Configurable.h"
#include <memory>
#include "Core/Exception.h"
#include "Core/AutoPtr.h"
namespace PH {
class Message;
class  Formatter: public Configurable
{
public:
    using Ptr = AutoPtr<Formatter>;

    Formatter();

    virtual ~Formatter();

    virtual void format(const Message& msg, std::string& text) = 0;

    void setProperty(const std::string& name, const std::string& value);

    std::string getProperty(const std::string& name) const;
};
}
#endif // FORMATTER_H
