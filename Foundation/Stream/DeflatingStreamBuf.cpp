﻿#include "DeflatingStreamBuf.h"
#include "Core/Exception.h"
namespace PH {

DeflatingStreamBuf::DeflatingStreamBuf(std::istream &istr, DeflatingStreamBuf::StreamType type, int level):
    BufferedStreamBuf(STREAM_BUFFER_SIZE,std::ios::in),
    _pIstr(&istr),
    _pOstr(nullptr),
    _eof(false)
{
    _zstr.next_in=0;
    _zstr.avail_in=0;
    _zstr.total_in=0;
    _zstr.next_out=0;
    _zstr.avail_out=0;
    _zstr.total_out=0;
    _zstr.msg=0;
    _zstr.state=0;
    _zstr.zalloc=Z_NULL;
    _zstr.zfree=Z_NULL;
    _zstr.opaque=Z_NULL;
    _zstr.data_type=0;
    _zstr.adler=0;
    _zstr.reserved=0;

    _buffer=new char[DEFLATE_BUFFER_SIZE];
    int rc=deflateInit2(&_zstr,level,Z_DEFLATED,15+(type==STREAM_GZIP?16:0),8,Z_DEFAULT_STRATEGY);
    if(Z_OK!=rc){
        delete []_buffer;
        throw PH::Exception(zError(rc));
    }
}

DeflatingStreamBuf::DeflatingStreamBuf(std::istream &istr, int windowsBits, int level):
    BufferedStreamBuf(STREAM_BUFFER_SIZE,std::ios::in),
    _pIstr(&istr),
    _pOstr(nullptr),
    _eof(false)
{
    _zstr.zalloc=Z_NULL;
    _zstr.zfree=Z_NULL;
    _zstr.opaque=Z_NULL;
    _zstr.next_in=0;
    _zstr.avail_in=0;
    _zstr.next_out=0;
    _zstr.avail_out=0;

    _buffer=new char[DEFLATE_BUFFER_SIZE];

    int rc=deflateInit2(&_zstr,level,Z_DEFLATED,windowsBits,8,Z_DEFAULT_STRATEGY);
    if(Z_OK!=rc){
        delete [] _buffer;
        throw PH::Exception(zError(rc));
    }
}

DeflatingStreamBuf::DeflatingStreamBuf(std::ostream &ostr, DeflatingStreamBuf::StreamType type, int level):
    BufferedStreamBuf(STREAM_BUFFER_SIZE,std::ios::out),
    _pIstr(nullptr),
    _pOstr(&ostr),
    _eof(false)
{
    _zstr.zalloc=Z_NULL;
    _zstr.zfree=Z_NULL;
    _zstr.opaque=Z_NULL;
    _zstr.next_in=0;
    _zstr.avail_in=0;
    _zstr.next_out=0;
    _zstr.avail_out=0;

    _buffer=new char[DEFLATE_BUFFER_SIZE];
    int rc=deflateInit2(&_zstr,level,Z_DEFLATED,15+(type==STREAM_GZIP?16:0),8,Z_DEFAULT_STRATEGY);
    if(Z_OK!=rc){
        delete []_buffer;
        throw PH::Exception(zError(rc));
    }

}

DeflatingStreamBuf::DeflatingStreamBuf(std::ostream &ostr, int windowBits, int level):
    BufferedStreamBuf(STREAM_BUFFER_SIZE,std::ios::out),
    _pIstr(nullptr),
    _pOstr(&ostr),
    _eof(false)
{
    _zstr.zalloc=Z_NULL;
    _zstr.zfree=Z_NULL;
    _zstr.opaque=Z_NULL;
    _zstr.next_in=0;
    _zstr.avail_in=0;
    _zstr.next_out=0;
    _zstr.avail_out=0;

    _buffer=new char[DEFLATE_BUFFER_SIZE];

    int rc=deflateInit2(&_zstr,level,Z_DEFLATED,windowBits,8,Z_DEFAULT_STRATEGY);
    if(Z_OK!=rc){
        delete [] _buffer;
        throw PH::Exception(zError(rc));
    }
}

DeflatingStreamBuf::~DeflatingStreamBuf()
{
    try{
        close();
    }catch(...){

    }

    delete [] _buffer;
    deflateEnd(&_zstr);
}

int DeflatingStreamBuf::close()
{
    BufferedStreamBuf::sync();
    _pIstr=0;
    if(!_pOstr) return 0;
    if(!_zstr.next_out){
        _pOstr->flush();
        _pOstr=0;
        return 0;
    }

    int rc=deflate(&_zstr,Z_FILTERED);
    if(rc!=Z_OK&&rc!=Z_STREAM_ERROR) throw PH::Exception(zError(rc));

    _pOstr->write(_buffer,DEFLATE_BUFFER_SIZE-_zstr.avail_out);
    if(!_pOstr->good()) throw PH::Exception("Failed writing deflated data to output stream");
    _zstr.next_out=(unsigned char*) _buffer;
    _zstr.avail_out=DEFLATE_BUFFER_SIZE;

    while (Z_STREAM_END!=rc) {
        rc=deflate(&_zstr,Z_FINISH);
        if(rc!=Z_OK&&rc!=Z_STREAM_END) throw PH::Exception(zError(rc));
        _pOstr->write(_buffer,DEFLATE_BUFFER_SIZE-_zstr.avail_out);
        if(!_pOstr->good()) throw PH::Exception("Failed writing deflated data to output stream");
        _zstr.next_out=(unsigned char*)_buffer;
        _zstr.avail_out=DEFLATE_BUFFER_SIZE;
    }

    _pOstr->flush();
    _pOstr=0;
    return 0;
}

int DeflatingStreamBuf::readFromDevice(BasicBufferedStreamBuf::char_type * buffer, std::streamsize length)
{
    if(!_pIstr) return 0;

    if(_zstr.avail_in==0&&!_eof) {
        int n=0;
        if(_pIstr->good()){
            _pIstr->read(_buffer,DEFLATE_BUFFER_SIZE);
            n=static_cast<int>(_pIstr->gcount());
        }

        if(n>0){
            _zstr.next_in=(unsigned char*) _buffer;
            _zstr.avail_in=n;
        }else{
            _zstr.next_in=0;
            _zstr.avail_in=0;
            _eof=true;
        }
    }

    _zstr.next_out=(unsigned char*)buffer;
    _zstr.avail_out=static_cast<unsigned>(length);
    for(;;){
        int rc=deflate(&_zstr,_eof?Z_FINISH:Z_NO_FLUSH);
        if(_eof&&rc==Z_STREAM_END){
            _pIstr=0;
            return static_cast<int>(length)-_zstr.avail_out;
        }

        if(Z_OK!=rc) PH::Exception(zError(rc));
        if(0==_zstr.avail_out) return static_cast<int>(length);

        if(0!=_zstr.avail_in) continue;
        int n=0;
        if(_pIstr->good()){
            _pIstr->read(_buffer,DEFLATE_BUFFER_SIZE);
            n=static_cast<int>(_pIstr->gcount());
        }

        if(n>0){
            _zstr.next_in=(unsigned char*)_buffer;
            _zstr.avail_in=n;
            continue;
        }

        _zstr.next_in=0;
        _zstr.avail_in=0;
        _eof=true;
    }
}

int DeflatingStreamBuf::writeToDevice(const BasicBufferedStreamBuf::char_type * buffer, std::streamsize length)
{
    if(0==length||!_pOstr) return 0;
    _zstr.next_in=(unsigned char*)buffer;
    _zstr.avail_in=static_cast<unsigned>(length);
    _zstr.next_out=(unsigned char*)_buffer;
    _zstr.avail_out=DEFLATE_BUFFER_SIZE;

    for(;;){
        int rc=deflate(&_zstr,Z_NO_FLUSH);
        if(Z_OK!=rc) throw PH::Exception(zError(rc));

        if(0==_zstr.avail_out){
            _pOstr->write(_buffer,DEFLATE_BUFFER_SIZE);
            if(_pOstr->good()) throw PH::Exception("Failed writing deflated data to output stream");
            _zstr.next_out=(unsigned char*)_buffer;
            _zstr.avail_out=DEFLATE_BUFFER_SIZE;
        }

        if(0!=_zstr.avail_in) continue;

        _pOstr->write(_buffer,DEFLATE_BUFFER_SIZE-_zstr.avail_out);
        if(!_pOstr->good()) throw PH::Exception("Failed writing deflated data to output stream");
        _zstr.next_out=(unsigned char*)_buffer;
        _zstr.avail_out=DEFLATE_BUFFER_SIZE;
        break;
    }

    return static_cast<int>(length);
}

int DeflatingStreamBuf::sync()
{
    if(BufferedStreamBuf::sync()) return -1;

    if(!_pOstr) return 0;

    if(!_zstr.next_out) return 0;

    int rc=deflate(&_zstr,Z_SYNC_FLUSH);
    if(Z_OK!=rc) throw PH::Exception(zError(rc));
    _pOstr->write(_buffer,DEFLATE_BUFFER_SIZE-_zstr.avail_out);
    if(!_pOstr->good()) throw PH::Exception("Failed writing deflated data to output stream");

    while (0==_zstr.avail_out) {
        _zstr.next_out=(unsigned char*)_buffer;
        _zstr.avail_out=DEFLATE_BUFFER_SIZE;
        rc=deflate(&_zstr,Z_SYNC_FLUSH);
        if(Z_OK!=rc) PH::Exception(zError(rc));

        _pOstr->write(_buffer,DEFLATE_BUFFER_SIZE-_zstr.avail_out);
        if(!_pOstr->good()) throw PH::Exception("Failed writing deflated data to output stream");
    }

    _zstr.next_out=(unsigned char*)_buffer;
    _zstr.avail_out=DEFLATE_BUFFER_SIZE;
    return 0;
}

DeflatingIOS::DeflatingIOS(std::ostream &ostr, DeflatingStreamBuf::StreamType type, int level):
    _buf(ostr,type,level)
{

}

DeflatingIOS::DeflatingIOS(std::ostream &ostr, int windowBits, int level):
    _buf(ostr,windowBits,level)
{

}

DeflatingIOS::DeflatingIOS(std::istream &istr, DeflatingStreamBuf::StreamType type, int level):
    _buf(istr,type,level)
{

}

DeflatingIOS::DeflatingIOS(std::istream &istr, int windowBits, int level):
    _buf(istr,windowBits,level)
{

}

DeflatingIOS::~DeflatingIOS()
{

}

DeflatingStreamBuf *DeflatingIOS::rdbuf()
{
    return &_buf;
}

DeflatingOutputStream::DeflatingOutputStream(std::ostream &ostr, DeflatingStreamBuf::StreamType type, int level):
    std::ostream(&_buf),
    DeflatingIOS(ostr,type,level)
{

}

DeflatingOutputStream::DeflatingOutputStream(std::ostream &ostr, int windowBits, int level):
    std::ostream(&_buf),
    DeflatingIOS(ostr,windowBits,level)
{

}

DeflatingOutputStream::~DeflatingOutputStream()
{

}

int DeflatingOutputStream::close()
{
    return _buf.close();
}

int DeflatingOutputStream::sync()
{
    return _buf.pubsync();
}

DeflatingInputStream::DeflatingInputStream(std::istream &istr, DeflatingStreamBuf::StreamType type, int level):
    std::istream(&_buf),
    DeflatingIOS(istr,type,level)
{

}

DeflatingInputStream::DeflatingInputStream(std::istream &istr, int windowBits, int level):
    std::istream(&_buf),
    DeflatingIOS(istr,windowBits,level)
{

}

DeflatingInputStream::~DeflatingInputStream()
{

}



}
