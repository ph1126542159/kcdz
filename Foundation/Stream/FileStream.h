#ifndef FILESTREAM_H
#define FILESTREAM_H
#include <istream>
#include <ostream>
#include "StreamUtil.h"

#if defined(TEST_WIN32)
#include "FileStream_WIN32.h"
#else
#include "FileStream_POSIX.h"
#endif

namespace PH {

class FileIOS : public virtual std::ios{
public:
    FileIOS(std::ios::openmode defaultMode);

    ~FileIOS();

    void open(const std::string& path,std::ios::openmode mode);

    void close();

    FileStreamBuf* rdbuf();
protected:
    FileStreamBuf _buf;
    std::ios::openmode _defaultMode;
};

class FileInputStream : public FileIOS,public std::istream{
public:
    FileInputStream();

    ~FileInputStream();

    FileInputStream(const std::string& path,std::ios::openmode mode=std::ios::in);
};

class FileOutputStream : public FileIOS,public std::ostream{
  public:
    FileOutputStream();

    FileOutputStream(const std::string& path,std::ios::openmode mode=std::ios::out|std::ios::trunc);

    ~FileOutputStream();
};

class FileStream : public FileIOS,public std::iostream{
  public:
    FileStream();

    FileStream(const std::string& path,std::ios::openmode mode=std::ios::out|std::ios::in);

    ~FileStream();
};
}

#endif // FILESTREAM_H
