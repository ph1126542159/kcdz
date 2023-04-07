#ifndef STREAMCOPIER_H
#define STREAMCOPIER_H
#include <istream>
#include <ostream>
#include <cstddef>
#include "StreamUtil.h"

namespace PH {
class StreamCopier
{
public:
    static std::streamsize copyStream(std::istream& istr,std::ostream& ostr,std::size_t bufferSize=8192);
    static std::streamsize copyStreamUnbuffered(std::istream& istr,std::ostream& ostr);
    static std::streamsize copyToString(std::istream& istr,std::string& str,std::size_t bufferSize=8192);
#if defined(PH_HAVE_INT64)
    static PH::UInt64 copyStream64(std::istream& istr,std::ostream& ostr,std::size_t bufferSize=8192);
    static PH::UInt64 copyStreamUnbuffered64(std::istream& istr,std::ostream& ostr);
    static PH::UInt64 copyToString64(std::istream& istr,std::string& str,std::size_t bufferSize=8192);
#endif
};
}



#endif // STREAMCOPIER_H
