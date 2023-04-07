#include "FileStream_POSIX.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include "Core/Exception.h"

namespace PH {

FileStreamBuf::FileStreamBuf():
    BufferedBidirectionalStreamBuf(BUFFER_SIZE,std::ios::in|std::ios::out),
    _fd(-1),
    _pos(0){

}

FileStreamBuf::~FileStreamBuf(){
    close();
}

void FileStreamBuf::open(const std::string &path, std::ios::openmode mode){
    assert(_fd==-1);

    _pos=0;
    _path=path;
    setMode(mode);
    resetBuffers();

    int flags(0);
    if(mode&std::ios::trunc) flags|=O_TRUNC;
    if(mode&std::ios::app) flags|=O_APPEND;
    if(mode&std::ios::out) flags|=O_CREAT;
    if((mode&std::ios::in)&&(mode&std::ios::out)) flags|=O_RDWR;
    if(mode&std::ios::in) flags|=O_RDONLY;
    else flags|=O_WRONLY;

    _fd=::open(path.c_str(),flags,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if(-1==_fd) handleLastErrorImpl(errno,_path);

    if((mode&std::ios::app)||(mode&std::ios::ate)) seekoff(0,std::ios::end,mode);
}

int FileStreamBuf::readFromDevice(char *buffer, std::streamsize length){
    if(-1==_fd) return -1;
    if(getMode()&std::ios::out) sync();

    ssize_t n=read(_fd,buffer,static_cast<std::size_t>(length));
    if(-1==n) handleLastErrorImpl(errno,_path);
    _pos+=n;
    return static_cast<int>(n);
}

int FileStreamBuf::writeToDevice(const char *buffer, std::streamsize length){
    if(-1==_fd) return -1;

    ssize_t n=write(_fd,buffer,static_cast<std::size_t>(length));
    if(-1==n) handleLastErrorImpl(errno,_path);

    _pos+=n;
    return static_cast<int>(n);
}
bool FileStreamBuf::close(){
    bool success=true;
    if(-1==_fd) return success;
    try{
        sync();
    }catch(...){
        success=false;
    }
    ::close(_fd);
    _fd=-1;
    return success;
}

std::streampos FileStreamBuf::seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode){
    if(-1==_fd||!(getMode()&mode)) return -1;

    if(getMode()&std::ios::out) sync();

    std::streamoff adj=0;
    if(mode&std::ios::in) adj=static_cast<std::streamoff>(egptr()-gptr());

    resetBuffers();

    int whence=SEEK_SET;
    if(std::ios::cur==dir){
        whence=SEEK_CUR;
        off-=adj;
    }else if(std::ios::end==dir){
        whence=SEEK_END;
    }
    _pos=lseek(_fd,off,whence);
    return _pos;
}

std::streampos FileStreamBuf::seekpos(std::streampos pos, std::ios::openmode mode){
    if(-1==_fd||!(getMode()&mode)) return -1;

    if(getMode()&std::ios::out) sync();

    resetBuffers();

    _pos=lseek(_fd,pos,SEEK_SET);
    return _pos;
}
void FileStreamBuf::handleLastErrorImpl(int err, const std::string &path){
    switch (err) {
    case EIO:
        throw PH::Exception(path, err);
    case EPERM:
        throw PH::Exception("insufficient permissions", path, err);
    case EACCES:
        throw PH::Exception(path, err);
    case ENOENT:
        throw PH::Exception(path, err);
    case ENOTDIR:
        throw PH::Exception("not a directory", path, err);
    case EISDIR:
        throw PH::Exception("not a file", path, err);
    case EROFS:
        throw PH::Exception(path, err);
    case EEXIST:
        throw PH::Exception(path, err);
    case ENOSPC:
        throw PH::Exception("no space left on device", path, err);
    case ENAMETOOLONG:
        throw PH::Exception(path, err);
    case EMFILE:
        throw PH::Exception("too many open files", path, err);
    default:
        throw PH::Exception("unknow", path, err);
    }
}
}
