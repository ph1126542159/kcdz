#include <Windows.h>
#include "FileStream_WIN32.h"
#include <assert.h>
#include <string>
namespace PH {

FileStreamBuf::FileStreamBuf():
    BasicBufferedBidirectionalStreamBuf(BUFFER_SIZE,std::ios::in|std::ios::out),
    _handle(INVALID_HANDLE_VALUE),
    _pos(0)
{

}

FileStreamBuf::~FileStreamBuf()
{
    close();
}

void FileStreamBuf::open(const std::string &path, std::ios_base::openmode mode)
{
    assert(_handle==INVALID_HANDLE_VALUE);
    _path=path;
    _pos=0;
    setMode(mode);
    resetBuffers();
    DWORD acces=0;
    if(mode&std::ios::in) acces|=GENERIC_READ;
    if(mode&std::ios::out) acces|=GENERIC_WRITE;

    DWORD sharemode=FILE_SHARE_READ;
    if(mode&std::ios::out) sharemode|=FILE_SHARE_WRITE;

    DWORD creatDisp=OPEN_EXISTING;
    if(mode&std::ios::trunc) creatDisp=CREATE_ALWAYS;
    else if(mode&std::ios::out) creatDisp=OPEN_ALWAYS;

    DWORD flags=FILE_ATTRIBUTE_NORMAL;
    _handle=CreateFileA(_path.c_str(),acces,sharemode,NULL,creatDisp,flags,NULL);
    if(INVALID_HANDLE_VALUE==_handle){
        handleLastError();
    }

    if((mode&std::ios::ate)||(mode&std::ios::app)){
        seekoff(0,std::ios::end,mode);
    }
}

bool FileStreamBuf::close()
{
    if(_handle==INVALID_HANDLE_VALUE) return true;
    bool succes=true;
    try{
        if(getMode()&std::ios::out) sync();
    }catch(...){
        succes=false;
    }
    CloseHandle(_handle);
    _handle=INVALID_HANDLE_VALUE;
    return succes;
}

std::streampos FileStreamBuf::seekoff(std::streamoff off, std::ios_base::seekdir dir, std::ios_base::openmode mode)
{
    if(INVALID_HANDLE_VALUE==_handle||!(mode&getMode())) return -1;
    if(getMode()&std::ios::out) sync();

    std::streamoff adj;
    if(mode&std::ios::in) adj=static_cast<std::streamoff>(egptr()-gptr());
    else adj=0;

    resetBuffers();

    DWORD offset=FILE_BEGIN;
    if(dir==std::ios::cur){
        offset=FILE_CURRENT;
        off-=adj;
    }else if(dir==std::ios::end){
        offset=FILE_END;
    }

    LARGE_INTEGER li;
    li.QuadPart=off;
    li.LowPart=SetFilePointer(_handle,li.LowPart,&li.HighPart,offset);

    if(li.LowPart==INVALID_SET_FILE_POINTER&&GetLastError()!=NO_ERROR)
        handleLastError();

    _pos=li.QuadPart;
    return std::streampos(static_cast<std::streamoff>(_pos));
}

std::streampos FileStreamBuf::seekpos(std::streampos pos, std::ios_base::openmode mode)
{
    if(INVALID_HANDLE_VALUE==_handle||!(mode&getMode())) return -1;
    if(getMode()&std::ios::out) sync();

    resetBuffers();

    LARGE_INTEGER li;
    li.QuadPart=pos;
    li.LowPart=SetFilePointer(_handle,li.LowPart,&li.HighPart,FILE_BEGIN);

    if(li.LowPart==INVALID_SET_FILE_POINTER&&GetLastError()!=NO_ERROR)
        handleLastError();

    _pos=li.QuadPart;
    return std::streampos(static_cast<std::streamoff>(_pos));
}

int FileStreamBuf::readFromDevice(BasicBufferedBidirectionalStreamBuf::char_type *buffer, std::streamsize length)
{
    if(INVALID_HANDLE_VALUE==_handle&&!(getMode()&std::ios::in)) return -1;
    if(getMode()&std::ios::out) sync();

    DWORD readBytes(0);
    BOOL rc=ReadFile(_handle,buffer,static_cast<DWORD>(length),&readBytes,NULL);
    if(0==rc) handleLastError();
    _pos+=readBytes;
    return static_cast<int>(readBytes);
}

int FileStreamBuf::writeToDevice(const BasicBufferedBidirectionalStreamBuf::char_type *buffer, std::streamsize length)
{
    if(INVALID_HANDLE_VALUE==_handle&&!(getMode()&std::ios::out)) return -1;
    if(getMode()&std::ios::app) {
        LARGE_INTEGER li;
        li.QuadPart=0;
        li.LowPart=SetFilePointer(_handle,li.LowPart,&li.HighPart,FILE_END);
        if(GetLastError()!=NO_ERROR||INVALID_SET_FILE_POINTER==li.LowPart) handleLastError();
        _pos=li.QuadPart;
    }

    DWORD bytesWrite(0);
    BOOL rc=WriteFile(_handle,buffer,static_cast<DWORD>(length),&bytesWrite,NULL);
    if(0==rc) handleLastError();

    _pos+=bytesWrite;
    return static_cast<int>(bytesWrite);
}

void FileStreamBuf::exception(const char *info, const std::string &path, int err)
{
    std::string infoStr(info);
    infoStr+="path:";
    infoStr+=path;
    infoStr+="errno num:";
    infoStr+=std::to_string(err);

    throw infoStr.c_str();
}

void FileStreamBuf::handleLastError()
{
    std::string path=_path;
    DWORD err = GetLastError();
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        exception("ERROR_FILE_NOT_FOUND",path, err);
    case ERROR_PATH_NOT_FOUND:
    case ERROR_BAD_NETPATH:
    case ERROR_CANT_RESOLVE_FILENAME:
    case ERROR_INVALID_DRIVE:
        exception("ERROR_INVALID_DRIVE",path, err);
    case ERROR_ACCESS_DENIED:
        exception("ERROR_ACCESS_DENIED",path, err);
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:
        exception("ERROR_FILE_EXISTS",path, err);
    case ERROR_INVALID_NAME:
    case ERROR_DIRECTORY:
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_BAD_PATHNAME:
        exception("ERROR_BAD_PATHNAME",path, err);
    case ERROR_FILE_READ_ONLY:
        exception("ERROR_FILE_READ_ONLY",path, err);
    case ERROR_CANNOT_MAKE:
        exception("ERROR_CANNOT_MAKE",path, err);
    case ERROR_DIR_NOT_EMPTY:
        exception("ERROR_DIR_NOT_EMPTY",path, err);
    case ERROR_WRITE_FAULT:
        exception("ERROR_WRITE_FAULT",path, err);
    case ERROR_READ_FAULT:
        exception("ERROR_READ_FAULT",path, err);
    case ERROR_SHARING_VIOLATION:
        exception("sharing violation", path, err);
    case ERROR_LOCK_VIOLATION:
        exception("lock violation", path, err);
    case ERROR_HANDLE_EOF:
        exception("EOF reached", path, err);
    case ERROR_HANDLE_DISK_FULL:
    case ERROR_DISK_FULL:
        exception("disk is full", path, err);
    case ERROR_NEGATIVE_SEEK:
        exception("negative seek", path, err);
    default:
        exception("unknow",path, err);
    }

}
}
