#ifndef LOGFILE_H
#define LOGFILE_H
#include <QTextStream>
#include <QDateTime>
#include <QFile>

namespace PH {


class   LogFile
{
public:
    LogFile(const std::string& path);

    ~LogFile();

    void write(const QString& text, bool flush = true);

    quint64 size() const;

    QString readLine();

    QDateTime creationDate() const;

    const std::string& path() const;
private:
    std::string _path;
    QDateTime _creationDate;
    QFile   _file;
    mutable QTextStream _str;
};
}

#endif // LOGFILE_H
