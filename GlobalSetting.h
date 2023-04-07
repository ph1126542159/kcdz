#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include "Core/Singleholder.h"
#include "Core/SharedPtr.h"
#include <QSettings>
class GlobalSetting : public QObject,public PH::SingleHolder<GlobalSetting>
{
    Q_OBJECT
public:
    ///system logger group
    QString getLoggerName(const QString& defValue="appLogger");
    QString getLoggerPath(const QString& defValue="./logger/appLogger.log");
    bool getEnableConsle(bool defValue=false);
    bool getEnableFile(bool defValue=false);
private:
    friend class PH::SingleHolder<GlobalSetting>;
    explicit GlobalSetting(QObject *parent = nullptr);
    PH::SharedPtr<QSettings> _ptrSetting;
    template<typename T>
    T getValue(const T& value,const QString& key,const QString& group="");
};

#endif // GLOBALSETTING_H
