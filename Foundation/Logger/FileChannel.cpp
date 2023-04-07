#include "FileChannel.h"
#include "RotateStrategy.h"
#include "PurgeStrategy.h"
#include "ArchiveStrategy.h"
#include "Message.h"
#include <ctype.h>

namespace PH {
const std::string FileChannel::PROP_PATH         = "path";
const std::string FileChannel::PROP_ROTATION     = "rotation";
const std::string FileChannel::PROP_ARCHIVE      = "archive";
const std::string FileChannel::PROP_TIMES        = "times";
const std::string FileChannel::PROP_COMPRESS     = "compress";
const std::string FileChannel::PROP_PURGEAGE     = "purgeAge";
const std::string FileChannel::PROP_PURGECOUNT   = "purgeCount";
const std::string FileChannel::PROP_FLUSH        = "flush";
const std::string FileChannel::PROP_ROTATEONOPEN = "rotateOnOpen";

const qint64 MILLISECONDS = 1000;
const qint64 SECONDS      = 1000*MILLISECONDS;
const qint64 MINUTES      =   60*SECONDS;
const qint64 HOURS        =   60*MINUTES;
const qint64 DAYS         =   24*HOURS;

FileChannel::FileChannel():
    _compress(false),
    _flush(true),
    _rotateOnOpen(false),
    _pFile(nullptr),
    _pRotateStrategy(nullptr),
    _pPurgeStrategy(nullptr),
    _pArchiveStrategy(new ArchiveByNumberStrategy)
{
}

FileChannel::FileChannel(const std::string& path):
    _path(path),
    _compress(false),
    _flush(true),
    _rotateOnOpen(false),
    _pFile(nullptr),
    _pRotateStrategy(nullptr),
    _pPurgeStrategy(nullptr),
    _pArchiveStrategy(new ArchiveByNumberStrategy)
{
}


FileChannel::~FileChannel()
{
    try
    {
        close();
        delete _pRotateStrategy;
        delete _pArchiveStrategy;
        delete _pPurgeStrategy;
    }
    catch (...)
    {
    }
}

void FileChannel::open()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if(_pFile) return;
    _pFile = new LogFile(_path);
    if (_rotateOnOpen && _pFile->size() > 0)
    {
        try
        {
            _pFile = _pArchiveStrategy->archive(_pFile);
            purge();
        }
        catch (...)
        {
            _pFile = new LogFile(_path);
        }
    }
}


void FileChannel::close()
{
    std::lock_guard<std::mutex> lock(_mutex);

    delete _pFile;
    _pFile = nullptr;
}

void FileChannel::log(const Message& msg)
{
    open();

    std::lock_guard<std::mutex> lock(_mutex);

    if (_pRotateStrategy  && _pRotateStrategy->mustRotate(_pFile))
    {
        try
        {
            //重命名和压缩 存档策略
            _pFile = _pArchiveStrategy->archive(_pFile);
            //删除策略
            purge();
        }
        catch (...)
        {
            _pFile = new LogFile(_path);
        }
        _pRotateStrategy->mustRotate(_pFile);
    }
    _pFile->write(msg.getText().c_str(), _flush);
}

void FileChannel::setProperty(const std::string& name, const std::string& value)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (name == PROP_TIMES)
    {
        if (!_rotation.empty())
            setRotation(_rotation);

        if (!_archive.empty())
            setArchive(_archive);
    }else if (name == PROP_PATH)
        _path = value;
    else if (name == PROP_ROTATION)
        setRotation(value);
    else if (name == PROP_ARCHIVE){
        setArchive(value);
    }
    else if (name == PROP_COMPRESS){
        setCompress(value);
    }
    else if (name == PROP_PURGEAGE)
        setPurgeAge(value);
    else if (name == PROP_PURGECOUNT)
        setPurgeCount(value);
    else if (name == PROP_FLUSH)
        setFlush(value);
    else if (name == PROP_ROTATEONOPEN)
        setRotateOnOpen(value);
    else
        Channel::setProperty(name, value);
}

std::string FileChannel::getProperty(const std::string& name) const
{
    if (name == PROP_TIMES)
        return "local";
    else if (name == PROP_PATH)
        return _path;
    else if (name == PROP_ROTATION)
        return _rotation;
    else if (name == PROP_ARCHIVE){
        return _archive;
    }
    else if (name == PROP_COMPRESS){
        return std::string(_compress ? "true" : "false");
    }
    else if (name == PROP_PURGEAGE)
        return _purgeAge;
    else if (name == PROP_PURGECOUNT)
        return _purgeCount;
    else if (name == PROP_FLUSH)
        return std::string(_flush ? "true" : "false");
    else if (name == PROP_ROTATEONOPEN)
        return std::string(_rotateOnOpen ? "true" : "false");


    return Channel::getProperty(name);
}


QDateTime FileChannel::creationDate() const
{
    if (_pFile)
        return _pFile->creationDate();
    else
        return QDateTime::fromMSecsSinceEpoch(0);
}


quint64 FileChannel::size() const
{
    if (_pFile)
        return _pFile->size();
    else
        return 0;
}


const std::string& FileChannel::path() const
{
    return _path;
}
void FileChannel::setRotation(const std::string& rotation)
{
    std::string::const_iterator it  = rotation.begin();
    std::string::const_iterator end = rotation.end();
    int n = 0;
    while (it != end && isspace(*it)) ++it;
    while (it != end && isdigit(*it)) { n *= 10; n += *it++ - '0'; }
    while (it != end && isspace(*it)) ++it;
    std::string unit;
    while (it != end && isalpha(*it)) unit += *it++;

    RotateStrategy* pStrategy = nullptr;
    if ((rotation.find(',') != std::string::npos) || (rotation.find(':') != std::string::npos))
    {
        pStrategy = new RotateAtTimeStrategy(rotation.c_str());
    }
    else if (unit == "daily")
        pStrategy = new RotateByIntervalStrategy(QDateTime(QDate(0,0,1)));
    else if (unit == "weekly")
        pStrategy = new RotateByIntervalStrategy(QDateTime(QDate(0,0,7)));
    else if (unit == "monthly")
        pStrategy = new RotateByIntervalStrategy(QDateTime(QDate(0,0,30)));
    else if (unit == "seconds")
        pStrategy = new RotateByIntervalStrategy(QDateTime::fromMSecsSinceEpoch(n*SECONDS));
    else if (unit == "minutes")
        pStrategy = new RotateByIntervalStrategy(QDateTime::fromMSecsSinceEpoch(n*MINUTES));
    else if (unit == "hours")
        pStrategy = new RotateByIntervalStrategy(QDateTime::fromMSecsSinceEpoch(n*HOURS));
    else if (unit == "days")
        pStrategy = new RotateByIntervalStrategy(QDateTime::fromMSecsSinceEpoch(n*DAYS));
    else if (unit == "weeks")
        pStrategy = new RotateByIntervalStrategy(QDateTime::fromMSecsSinceEpoch(7*n*SECONDS));
    else if (unit == "months")
        pStrategy = new RotateByIntervalStrategy(QDateTime::fromMSecsSinceEpoch(30*n*SECONDS));
    else if (unit == "K")
        pStrategy = new RotateBySizeStrategy(static_cast<quint64>(n*1024));
    else if (unit == "M")
        pStrategy = new RotateBySizeStrategy(static_cast<quint64>(n*1024*1024));
    else if (unit.empty())
        pStrategy = new RotateBySizeStrategy(static_cast<quint64>(n));
    else if (unit != "never")
        throw PH::Exception("Invalid Argument Exception");

    delete _pRotateStrategy;
    _pRotateStrategy = pStrategy;

    _rotation = rotation;
}
void FileChannel::setArchive(const std::string& archive)
{
    ArchiveStrategy* pStrategy = nullptr;
    if (archive == "number")
    {
        pStrategy = new ArchiveByNumberStrategy;
    }
    else if (archive == "timestamp")
    {
        pStrategy = new ArchiveByTimestampStrategy();
    }
    else throw PH::Exception("Invalid Argument Exception archive"+ archive);
    delete _pArchiveStrategy;
    pStrategy->compress(_compress);
    _pArchiveStrategy = pStrategy;
    _archive = archive;
}
void FileChannel::setCompress(const std::string& compress)
{
    _compress = compress.compare("true") == 0;
    if (_pArchiveStrategy)
        _pArchiveStrategy->compress(_compress);
}
quint64 FileChannel::extractFactor(const std::string& value, std::string::const_iterator start) const
{
    while (start != value.end() && isspace(*start)) ++start;

    std::string unit;
    while (start != value.end() && isalpha(*start)) unit += *start++;

    if (unit == "seconds")
        return SECONDS;
    if (unit == "minutes")
        return MINUTES;
    else if (unit == "hours")
        return HOURS;
    else if (unit == "days")
        return DAYS;
    else if (unit == "weeks")
        return 7 * DAYS;
    else if (unit == "months")
        return 30 * DAYS;
    else throw PH::Exception("Invalid Argument Exception"+value);
}

bool FileChannel::setNoPurge(const std::string& value)
{
    if (value.empty() || 0 == value.compare("none"))
    {
        delete _pPurgeStrategy;
        _pPurgeStrategy = nullptr;
        _purgeAge = "none";
        return true;
    }
    return false;
}
int FileChannel::extractDigit(const std::string& value, std::string::const_iterator* nextToDigit) const
{
    std::string::const_iterator it  = value.begin();
    std::string::const_iterator end = value.end();
    int digit 						= 0;

    while (it != end && isspace(*it)) ++it;
    while (it != end && isdigit(*it))
    {
        digit *= 10;
        digit += *it++ - '0';
    }

    if (digit == 0)
        throw PH::Exception("Invalid Argument Exception Zero is not valid purge age.");

    if (nextToDigit) *nextToDigit = it;
    return digit;
}
void FileChannel::setPurgeAge(const std::string& age)
{
    if (setNoPurge(age)) return;

    std::string::const_iterator nextToDigit;
    quint64 num =static_cast<quint64>(extractDigit(age, &nextToDigit)) ;
    quint64 factor = extractFactor(age, nextToDigit);
    setPurgeStrategy(new PurgeByAgeStrategy(num * factor/1000));
    _purgeAge = age;
}
void FileChannel::setPurgeStrategy(PurgeStrategy* strategy)
{
    delete _pPurgeStrategy;
    _pPurgeStrategy = strategy;
}
void FileChannel::setPurgeCount(const std::string& count)
{
    if (setNoPurge(count)) return;

    setPurgeStrategy(new PurgeByCountStrategy(static_cast<std::size_t>(extractDigit(count))));
    _purgeCount = count;
}


void FileChannel::setFlush(const std::string& flush)
{
    _flush = flush.compare("true") == 0;
}


void FileChannel::setRotateOnOpen(const std::string& rotateOnOpen)
{
    _rotateOnOpen = rotateOnOpen.compare("true") == 0;
}


void FileChannel::purge()
{
    if (_pPurgeStrategy)
    {
        try
        {
            _pPurgeStrategy->purge(_path.c_str());
        }
        catch (...)
        {
        }
    }
}
}
