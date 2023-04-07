#include "StreamCopier.h"
#include "Buffer.h"
#include <assert.h>


namespace PH {

std::streamsize StreamCopier::copyStream(std::istream &istr, std::ostream &ostr, std::size_t bufferSize)
{
    assert(bufferSize>0);
    Buffer<char> buffer(bufferSize);
    std::streamsize len=0;
    istr.read(buffer.begin(),bufferSize);
    std::streamsize n=istr.gcount();
    while (n>0) {
        len+=n;
        ostr.write(buffer.begin(),n);
        if(istr&&ostr){
            istr.read(buffer.begin(),bufferSize);
            n=istr.gcount();
        }else n=0;
    }

    return len;
}

std::streamsize StreamCopier::copyStreamUnbuffered(std::istream &istr, std::ostream &ostr)
{
    char c=0;
    std::streamsize len=0;
    istr.get(c);
    while (istr&&ostr) {
        ++len;ostr.put(c);istr.get(c);
    }
    return len;
}

std::streamsize StreamCopier::copyToString(std::istream &istr, std::string &str, std::size_t bufferSize)
{
    assert(bufferSize>0);
    Buffer<char> buffer(bufferSize);
    std::streamsize len=0;
    istr.read(buffer.begin(),bufferSize);
    std::streamsize n=istr.gcount();
    while (n>0) {
        len+=n;
        str.append(buffer.begin(),static_cast<std::string::size_type>(n));
        if(istr){
            istr.read(buffer.begin(),bufferSize);
            n=istr.gcount();
        }else n=0;
    }

    return len;
}
#if defined(PH_HAVE_INT64)
UInt64 StreamCopier::copyStream64(std::istream &istr, std::ostream &ostr, std::size_t bufferSize)
{
    assert(bufferSize>0);
    Buffer<char> buffer(bufferSize);
    PH::UInt64 len=0;
    istr.read(buffer.begin(),bufferSize);
    std::streamsize n=istr.gcount();

    while (n>0) {
        len+=n;
        ostr.write(buffer.begin(),n);
        if(istr&&ostr){
            istr.read(buffer.begin(),bufferSize);
            n=istr.gcount();
        }else n=0;
    }

    return len;
}

UInt64 StreamCopier::copyStreamUnbuffered64(std::istream &istr, std::ostream &ostr)
{
    char c=0;
    PH::UInt64 len=0;
    istr.get(c);
    while (istr&&ostr) {
        ++len;
        ostr.put(c);
        istr.get(c);
    }

    return len;
}

UInt64 StreamCopier::copyToString64(std::istream &istr, std::string &str, std::size_t bufferSize)
{
    assert(bufferSize>0);
    Buffer<char> buffer(bufferSize);
    PH::UInt64 len=0;
    istr.read(buffer.begin(),bufferSize);
    std::streamsize n=istr.gcount();
    while (n>0) {
        len+=n;
        str.append(buffer.begin(),static_cast<std::string::size_type>(n));
        if(istr){
            istr.read(buffer.begin(),bufferSize);
            n=istr.gcount();
        }else n=0;
    }

    return len;
}
#endif
}
