#include "GlobalSetting.h"
GlobalSetting::GlobalSetting(QObject *parent)
    : QObject{parent}
{
    _ptrSetting=new QSettings("./systemConfig.ini",QSettings::IniFormat);
}


QString GlobalSetting::getLoggerName(const QString& value)
{
    return getValue<QString>(value,"LoggerName","SystemLogger");
}

QString GlobalSetting::getLoggerPath(const QString &value)
{
    return getValue<QString>(value,"LoggerPath","SystemLogger");
}

bool GlobalSetting::getEnableConsle(bool defValue)
{
    return getValue<bool>(defValue,"EnableConsle","SystemLogger");
}

bool GlobalSetting::getEnableFile(bool defValue)
{
    return getValue<bool>(defValue,"EnableFile","SystemLogger");
}
template<typename T>
T GlobalSetting::getValue(const T& value,const QString& key,const QString& group){
    T result=value;
    _ptrSetting->beginGroup(group);
    if(_ptrSetting->contains(key)){
        result =_ptrSetting->value(key,value).template value<T>();
    }else{
        _ptrSetting->setValue(key,value);
    }
    _ptrSetting->endGroup();
    return result;
}
