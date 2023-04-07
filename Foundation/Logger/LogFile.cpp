#include "LogFile.h"
#include <QFileInfo>
#include <QDir>
namespace PH {

LogFile::LogFile(const std::string &path)
    :_path(path)
{
    QFileInfo crt(_path.c_str());
    QDir().mkpath(crt.path());

    _file.setFileName(path.c_str());
    _file.open(QFile::ReadWrite | QIODevice::Text |QIODevice::Append);
    _str.setDevice(&_file);
    _str.setCodec("UTF-8");
//    _str.setFieldAlignment(QTextStream::AlignLeft);
//    _str.setPadChar(' ');

    QFileInfo info(_file);
    if(size()==0){
        _creationDate=info.lastModified();
    }else {
#ifdef Q_OS_WIN32
        _creationDate=info.created();
#elif defined Q_OS_LINUX
        _creationDate=info.birthTime();
#endif
    }
}

LogFile::~LogFile()
{
    _file.close();
}

void LogFile::write(const QString &text, bool flush)
{
    _str<<text<<"\n";
    if(flush){
        _str.flush();
    }else{
        _str<<"\n";
    }
}

quint64 LogFile::size() const
{
    return static_cast<quint64>(_file.size());
}

QString LogFile::readLine()
{
    return _str.readLine();
}

QDateTime LogFile::creationDate() const
{
    return _creationDate;
}

const std::string &LogFile::path() const
{
    return _path;
}

}
