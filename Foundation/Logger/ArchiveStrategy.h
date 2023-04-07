#ifndef ARCHIVESTRATEGY_H
#define ARCHIVESTRATEGY_H

#include "LogFile.h"

namespace PH {
class ArchiveCompressor;
class  ArchiveStrategy{
public:
    ArchiveStrategy();

    virtual ~ArchiveStrategy();

    virtual LogFile* archive(LogFile* pFile) = 0;

    void compress(bool flag = true);

protected:
    void moveFile(const std::string& oldName, const std::string& newName);
    bool exists(const std::string& name);
private:
    ArchiveStrategy(const ArchiveStrategy&)=delete;
    ArchiveStrategy& operator = (const ArchiveStrategy&)=delete;

    bool _compress;
    ArchiveCompressor* _pCompresor;
};


class ArchiveByNumberStrategy : public ArchiveStrategy{
public:
    ArchiveByNumberStrategy();
    ~ArchiveByNumberStrategy();
    LogFile* archive(LogFile* pFile);
};

class ArchiveByTimestampStrategy :public ArchiveStrategy{
public:
    ArchiveByTimestampStrategy()
    {
    }

    ~ArchiveByTimestampStrategy()
    {
    }

    LogFile* archive(LogFile* pFile);

private:
    void archiveByNumber(const std::string& basePath);
};

}

#endif // ARCHIVESTRATEGY_H
