#include "Formatter.h"


namespace PH {


Formatter::Formatter()
{
}


Formatter::~Formatter()
{
}


void Formatter::setProperty(const std::string& /*name*/, const std::string& /*value*/)
{
    throw PH::Exception("Property Not Supported Exception");
}


std::string Formatter::getProperty(const std::string& /*name*/) const
{
    throw PH::Exception("Property Not Supported Exception");
}


}
