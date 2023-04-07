#ifndef SYSTEMLOGGER_H
#define SYSTEMLOGGER_H

#include <QObject>
#include "Logger/Logger.h"
#include "Core/SharedPtr.h"

namespace kcdz {
class SystemLoggerPrivate;
class SystemLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString loggerName READ loggerName WRITE setLoggerName)
    Q_PROPERTY(QString loggerPath READ loggerPath WRITE setLoggerPath)
    Q_PROPERTY(QString rotation READ rotation WRITE setRotation)
    Q_PROPERTY(QString archive READ archive WRITE setArchive)
    Q_PROPERTY(QString compress READ compress WRITE setCompress)
    Q_PROPERTY(QString loggerFormat READ loggerFormat WRITE setLoggerFormat)
    Q_PROPERTY(QString purgeAge READ purgeAge WRITE setPurgeAge)
    Q_PROPERTY(int loggerLevel READ loggerLevel WRITE setLoggerLevel)
    Q_PROPERTY(bool enableConsle READ enableConsle WRITE setEnableConsle)
    Q_PROPERTY(bool enableFile READ enableFile WRITE setEnableFile)
public:
    Q_INVOKABLE void init();
    Q_INVOKABLE void waitForFinished();

    explicit SystemLogger(QObject *parent = nullptr);

    virtual ~SystemLogger();

    void logMsg(QtMsgType type , const QMessageLogContext &context, const QString &msg);

    QString loggerName();

    void setLoggerName(const QString& name);

    QString loggerPath();

    void setLoggerPath(const QString& value);

    QString rotation();

    void setRotation(const QString& value);

    QString archive();

    void setArchive(const QString& value);

    QString compress();

    void setCompress(const QString& value);

    QString loggerFormat();

    void setLoggerFormat(const QString& value);

    QString purgeAge();

    void setPurgeAge(const QString& value);

    int loggerLevel();

    void setLoggerLevel(int value);

    bool enableConsle();

    void setEnableConsle(bool value);

    bool enableFile();

    void setEnableFile(bool value);
private:
    SystemLoggerPrivate* p;
};
}


#endif // SYSTEMLOGGER_H
