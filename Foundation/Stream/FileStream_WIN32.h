#ifndef FILESTREAM_WIN32_H
#define FILESTREAM_WIN32_H
#include "BasicBufferedBidirectionalStreamBuf.h"
#include "StreamUtil.h"
namespace PH {
class FileStreamBuf : public BufferedBidirectionalStreamBuf{
public:
    FileStreamBuf();
    ~FileStreamBuf();
    void open(const std::string& path,std::ios::openmode mode);
    bool close();
    std::streampos seekoff(std::streamoff off,std::ios::seekdir dir,std::ios::openmode mode=std::ios::in|std::ios::out);
    std::streampos seekpos(std::streampos pos,std::ios::openmode mode=std::ios::in|std::ios::out);
protected:
    enum{
        BUFFER_SIZE=4096
    };
    int readFromDevice(char_type *, std::streamsize)override;
    int writeToDevice(const char_type *, std::streamsize)override;
    void exception(const char* info,const std::string& path,int err);
private:
    void handleLastError();
    std::string _path;
    HANDLE _handle;
    UInt64 _pos;
};
}

#endif // FILESTREAM_WIN32_H
