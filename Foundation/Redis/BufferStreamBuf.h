#ifndef BUFFERSTREAMBUF_H
#define BUFFERSTREAMBUF_H
#include <streambuf>
#include <iosfwd>
#include <ios>
#include <cstddef>

#if !defined(PH_IOS_INIT_HACK)
// Microsoft Visual Studio with Dinkumware STL (but not STLport)
#	if defined(_MSC_VER) && (!defined(_STLP_MSVC) || defined(_STLP_NO_OWN_IOSTREAMS))
#		define PH_IOS_INIT_HACK 1
// QNX with Dinkumware but not GNU C++ Library
#	elif defined(__QNX__) && !defined(__GLIBCPP__)
#		define PH_IOS_INIT_HACK 1
// Linux with Clang libc++
#   elif defined(__linux) && defined(_LIBCPP_VERSION)
#		define PH_IOS_INIT_HACK 1
#	endif
#endif


#if defined(PH_IOS_INIT_HACK)
#	define ph_ios_init(buf)
#else
#	define ph_ios_init(buf) init(buf)
#endif

template<typename ch>
class BufferAllocator{
public:
    typedef ch char_type;

    static char_type* allocate(std::streamsize size)
    {
        return new char_type[static_cast<std::size_t>(size)];
    }

    static void deallocate(char_type* ptr, std::streamsize /*size*/) noexcept
    {
        delete [] ptr;
    }
};


template<typename ch,typename tr,typename ba=BufferAllocator<ch>>
class BasicBufferedStreamBuf: public std::basic_streambuf<ch,tr>{
protected:
    typedef std::basic_streambuf<ch, tr> Base;
    typedef std::basic_ios<ch, tr> IOS;
    typedef ch char_type;
    typedef tr char_traits;
    typedef ba Allocator;
    typedef typename Base::int_type int_type;
    typedef typename Base::pos_type pos_type;
    typedef typename Base::off_type off_type;
    typedef typename IOS::openmode openmode;
public:
    BasicBufferedStreamBuf(std::streamsize bufferSize, openmode mode):
        _bufsize(bufferSize),
        _pBuffer(Allocator::allocate(_bufsize)),
        _mode(mode)
    {
        this->setg(_pBuffer + 4, _pBuffer + 4, _pBuffer + 4);
        this->setp(_pBuffer, _pBuffer + _bufsize);
    }

    ~BasicBufferedStreamBuf()
    {
        try
        {
            Allocator::deallocate(_pBuffer, _bufsize);
        }
        catch (...)
        {
        }
    }

    virtual int_type overflow(int_type c)
    {
        if (!(_mode & IOS::out)) return char_traits::eof();

        if (flushBuffer() == std::streamsize(-1)) return char_traits::eof();
        if (c != char_traits::eof())
        {
            *this->pptr() = char_traits::to_char_type(c);
            this->pbump(1);
        }

        return c;
    }

    virtual int_type underflow()
    {
        if (!(_mode & IOS::in)) return char_traits::eof();

        if (this->gptr() && (this->gptr() < this->egptr()))
            return char_traits::to_int_type(*this->gptr());

        int putback = int(this->gptr() - this->eback());
        if (putback > 4) putback = 4;

        char_traits::move(_pBuffer + (4 - putback), this->gptr() - putback, putback);

        int n = readFromDevice(_pBuffer + 4, _bufsize - 4);
        if (n <= 0) return char_traits::eof();

        this->setg(_pBuffer + (4 - putback), _pBuffer + 4, _pBuffer + 4 + n);

        // return next character
        return char_traits::to_int_type(*this->gptr());
    }

    virtual int sync()
    {
        if (this->pptr() && this->pptr() > this->pbase())
        {
            if (flushBuffer() == -1) return -1;
        }
        return 0;
    }

protected:
    void setMode(openmode mode)
    {
        _mode = mode;
    }

    openmode getMode() const
    {
        return _mode;
    }
private:
    virtual int readFromDevice(char_type* /*buffer*/, std::streamsize /*length*/)
    {
        return 0;
    }

    virtual int writeToDevice(const char_type* /*buffer*/, std::streamsize /*length*/)
    {
        return 0;
    }

    int flushBuffer()
    {
        int n = int(this->pptr() - this->pbase());
        if (writeToDevice(this->pbase(), n) == n)
        {
            this->pbump(-n);
            return n;
        }
        return -1;
    }

    std::streamsize _bufsize;
    char_type*      _pBuffer;
    openmode        _mode;

    BasicBufferedStreamBuf(const BasicBufferedStreamBuf&)=delete;
    BasicBufferedStreamBuf& operator = (const BasicBufferedStreamBuf&)=delete;
};

typedef BasicBufferedStreamBuf<char, std::char_traits<char>> BufferedStreamBuf;

#endif // BUFFERSTREAMBUF_H
