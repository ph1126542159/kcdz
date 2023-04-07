#include "Configurable.h"
#include <typeinfo>
namespace PH {

Configurable::Configurable()
{

}

std::string Configurable::getName() const
{
    return typeid (*this).name();
}

Configurable::~Configurable()
{

}

}
