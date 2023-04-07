#ifndef CHANNEL_H
#define CHANNEL_H
#include "Configurable.h"
#include <memory>
#include "Core/Exception.h"
#include "Core/AutoPtr.h"
namespace PH {

class Message;

class   Channel: public Configurable
{
public:
    using Ptr = AutoPtr<Channel>;

    Channel();

    virtual void open();

    virtual void close();

    virtual void log(const Message& msg) = 0;

    void setProperty(const std::string& name, const std::string& ){
        throw PH::Exception("Property Not Supported Exception:"+name);
    }

    std::string getProperty(const std::string& name) const{
        throw PH::Exception("Property Not Supported Exception:"+name);
    }

protected:
    virtual ~Channel();

private:
    Channel(const Channel&)=delete;
    Channel& operator = (const Channel&)=delete;
};
}
#endif // CHANNEL_H
