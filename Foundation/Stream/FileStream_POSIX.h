#ifndef FILESTREAM_POSIX_H
#define FILESTREAM_POSIX_H

#include "BasicBufferedBidirectionalStreamBuf.h"
#include <istream>
#include <ostream>

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

    int readFromDevice(char* buufer,std::streamsize length);
    int writeToDevice(const char_type *, std::streamsize);
private:
    void handleLastErrorImpl(int err, const std::string& path);
    std::string _path;
    int _fd;
    std::streamoff _pos;
};
}

#endif // FILESTREAM_POSIX_H
