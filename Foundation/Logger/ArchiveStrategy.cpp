#include "ArchiveStrategy.h"
#include <QFile>
#include "Activetor/ActiveDispatcher.h"
#include "Activetor/ActiveMethod.h"
#include "Stream/DeflatingStreamBuf.h"
#include "Stream/StreamCopier.h"
#include "Stream/FileStream.h"
namespace PH {

class ArchiveCompressor : public PH::ActiveDispatcher{
public:
    ArchiveCompressor();
    virtual ~ArchiveCompressor();
    ActiveMethod<void,std::string,ArchiveCompressor> compress;
protected:
    void compressImpl(const std::string& path);
};

ArchiveCompressor::ArchiveCompressor():compress(this,&ArchiveCompressor::compressImpl){

}
ArchiveCompressor::~ArchiveCompressor(){

}
void ArchiveCompressor::compressImpl(const std::string& path){
    std::string gzPath(path);
    gzPath.append(".gz");
    PH::FileInputStream istr(path);
    PH::FileOutputStream ostr(gzPath);
    try{
        PH::DeflatingOutputStream deflater(ostr,PH::DeflatingStreamBuf::STREAM_GZIP);
        PH::StreamCopier::copyStream(istr,deflater);
        if(!deflater.good()||!ostr.good()) throw PH::Exception(gzPath,"create gz file failed.");

        deflater.close();
        ostr.close();
        istr.close();

    }catch(PH::Exception&){
        ostr.close();
        remove(gzPath.c_str());
    }

    remove(path.c_str());
}
ArchiveStrategy::ArchiveStrategy():
    _compress(false),
    _pCompresor(nullptr)
{

}

ArchiveStrategy::~ArchiveStrategy()
{
    if(nullptr!=_pCompresor)delete _pCompresor;
}

void ArchiveStrategy::compress(bool flag)
{
    _compress=flag;
}

void ArchiveStrategy::moveFile(const std::string &oldName, const std::string &newPath)
{
    bool compressed=false;
    QFile file(oldName.c_str());
    if(!file.exists()){
        file.setFileName(std::string(oldName+".gz").c_str());
        compressed=true;
    }
    std::string mvPath(newPath);
    if (_compress || compressed)
        mvPath.append(".gz");
    if (!_compress || compressed){
        file.rename(mvPath.c_str());
    }else{
        file.rename(newPath.c_str());
        if(!_pCompresor) _pCompresor=new ArchiveCompressor;
        _pCompresor->compress(newPath);
    }
}

bool ArchiveStrategy::exists(const std::string &name)
{
    QFile file(name.c_str());
    if(file.exists()) return true;

    if(_compress){
        std::string gzName(name);
        gzName.append(".gz");
        QFile gzf(gzName.c_str());
        return gzf.exists();
    }

    return false;
}

ArchiveByNumberStrategy::ArchiveByNumberStrategy()
{

}

ArchiveByNumberStrategy::~ArchiveByNumberStrategy()
{

}

LogFile *ArchiveByNumberStrategy::archive(LogFile *pFile)
{
    std::string basePath = pFile->path();
    delete pFile;
    int n = -1;
    std::string path;
    do
    {
        path = basePath;
        path.append(".");
        path.append(std::to_string(++n));
    }
    while (exists(path));

    while (n >= 0)
    {
        std::string oldPath = basePath;
        if (n > 0)
        {
            oldPath.append(".");
            oldPath.append(std::to_string(n - 1));
        }
        std::string newPath = basePath;
        newPath.append(".");
        newPath.append(std::to_string(n));
        moveFile(oldPath, newPath);
        --n;
    }
    return new LogFile(basePath);
}

LogFile *ArchiveByTimestampStrategy::archive(LogFile *pFile)
{
    std::string path = pFile->path();
    delete pFile;
    std::string archPath = path;
    archPath.append(".");
    QString tmStr=QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
    archPath+=tmStr.toStdString();
    if (exists(archPath)) archiveByNumber(archPath);
    else moveFile(path, archPath);

    return new LogFile(path);
}

void ArchiveByTimestampStrategy::archiveByNumber(const std::string &basePath)
{
    int n = -1;
    std::string path;
    do
    {
        path = basePath;
        path.append(".");
        path.append(std::to_string(++n));
    }
    while (exists(path));

    while (n >= 0)
    {
        std::string oldPath = basePath;
        if (n > 0)
        {
            oldPath.append(".");
            oldPath.append(std::to_string(n - 1));
        }
        std::string newPath = basePath;
        newPath.append(".");
        newPath.append(std::to_string(n));
        moveFile(oldPath, newPath);
        --n;
    }
}



}
