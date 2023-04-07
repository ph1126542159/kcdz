#ifndef FILECHANNEL_H
#define FILECHANNEL_H
#include "Channel.h"
#include <mutex>
#include <QDateTime>

namespace PH {
class LogFile;

//分割策略
class RotateStrategy;
//删除策略
class PurgeStrategy;
//文件重命名和压缩策略
class ArchiveStrategy;

class   FileChannel: public Channel{
public:
    FileChannel();

    FileChannel(const std::string& path);

    ~FileChannel();

    void open();

    void close();

    void log(const Message& msg);

    void setProperty(const std::string& name, const std::string& value);

    std::string getProperty(const std::string& name) const;

    QDateTime creationDate() const;

    quint64 size() const;

    const std::string& path() const;

    static const std::string PROP_PATH;
    static const std::string PROP_ROTATION;
    static const std::string PROP_ARCHIVE;
    static const std::string PROP_TIMES;
    static const std::string PROP_COMPRESS;
    static const std::string PROP_PURGEAGE;
    static const std::string PROP_PURGECOUNT;
    static const std::string PROP_FLUSH;
    static const std::string PROP_ROTATEONOPEN;
protected:
    void setRotation(const std::string& rotation);
    void setArchive(const std::string& archive);
    void setCompress(const std::string& compress);
    void setPurgeAge(const std::string& age);
    void setPurgeCount(const std::string& count);
    void setFlush(const std::string& flush);
    void setRotateOnOpen(const std::string& rotateOnOpen);
    void purge();

private:
    bool setNoPurge(const std::string& value);
    int extractDigit(const std::string& value, std::string::const_iterator* nextToDigit = nullptr) const;
    void setPurgeStrategy(PurgeStrategy* strategy);
    quint64 extractFactor(const std::string& value, std::string::const_iterator start) const;

    std::string      _path;
    std::string      _rotation;
    std::string      _archive;
    std::string      _purgeAge;
    bool             _compress;
    std::string      _purgeCount;
    bool             _flush;
    bool             _rotateOnOpen;
    LogFile*         _pFile;
    RotateStrategy*  _pRotateStrategy;
    PurgeStrategy*   _pPurgeStrategy;
    ArchiveStrategy* _pArchiveStrategy;
    std::mutex        _mutex;
};

}

#endif // FILECHANNEL_H
