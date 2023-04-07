#ifndef DEFLATINGSTREAMBUF_H
#define DEFLATINGSTREAMBUF_H
#include <istream>
#include <ostream>
#include "BufferedStreamBuf.h"
#include "zlib/zlib.h"


namespace PH {

class DeflatingStreamBuf : public BufferedStreamBuf{
public:
    enum StreamType{
        STREAM_ZLIB,
        STREAM_GZIP
    };

    DeflatingStreamBuf(std::istream& istr,StreamType type,int level);

    DeflatingStreamBuf(std::istream &istr, int windowsBits,int level);

    DeflatingStreamBuf(std::ostream& ostr,StreamType type,int level);

    DeflatingStreamBuf(std::ostream& ostr,int windowBits,int level);

    ~DeflatingStreamBuf();

    int close();

protected:
    int readFromDevice(char_type *, std::streamsize);
    int writeToDevice(const char_type *, std::streamsize);
    virtual int sync();

private:
    enum{
        STREAM_BUFFER_SIZE=1024,
        DEFLATE_BUFFER_SIZE=32768
    };

    std::istream* _pIstr;
    std::ostream* _pOstr;
    char* _buffer;
    z_stream _zstr;
    bool _eof;
};

class DeflatingIOS : public virtual std::ios{
public:
    DeflatingIOS(std::ostream &ostr, DeflatingStreamBuf::StreamType type=DeflatingStreamBuf::STREAM_ZLIB,int level=Z_DEFAULT_COMPRESSION);
    DeflatingIOS(std::ostream& ostr,int windowBits,int level);
    DeflatingIOS(std::istream &istr, DeflatingStreamBuf::StreamType type=DeflatingStreamBuf::STREAM_ZLIB,int level=Z_DEFAULT_COMPRESSION);
    DeflatingIOS(std::istream& istr,int windowBits,int level);
    ~DeflatingIOS();
    DeflatingStreamBuf* rdbuf();
protected:
    DeflatingStreamBuf _buf;
};

class DeflatingOutputStream : public std::ostream,public DeflatingIOS{
public:
    DeflatingOutputStream(std::ostream& ostr,DeflatingStreamBuf::StreamType type=DeflatingStreamBuf::STREAM_ZLIB,int level=Z_DEFAULT_COMPRESSION);
    DeflatingOutputStream(std::ostream &ostr, int windowBits,int level);

    ~DeflatingOutputStream();

    int close();
protected:
    virtual int sync();
};

class DeflatingInputStream : public std::istream,public DeflatingIOS{
public:
    DeflatingInputStream(std::istream& istr,DeflatingStreamBuf::StreamType type=DeflatingStreamBuf::STREAM_ZLIB,int level=Z_DEFAULT_COMPRESSION);
    DeflatingInputStream(std::istream &istr, int windowBits,int level);

    ~DeflatingInputStream();
};

}

#endif // DEFLATINGSTREAMBUF_H
