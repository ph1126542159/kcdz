#include "DBManager.h"
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QRunnable>


class TestWrite : public QRunnable{
public:
    void run()override{

    }
};

DBManager::DBManager(QObject *parent)
    : QObject{parent}
{

}
bool DBManager::init()
{
    _dbName="./db/data"+QDateTime::currentDateTime().toString("yyyy-MM")+".kcdz";
    if(!QFile::exists(_dbName)){
        if(!QFile::copy("./db/template/template.kcdz",_dbName)){
            qWarning()<<"sql db file copy failed."<<_dbName;
            return false;
        }
    }

    insertSqlOptional<Table::DET_PRODUCE>();
    qInfo()<<"open db succed,db name:"<<_dbName;
    return true;
}
template<class T>
void DBManager::insertSqlOptional()
{
    QString tableName=typeid(T).name();
    _ptrSqlOptional[tableName]=new Optional::TableOptional<T>();
    _ptrSqlOptional[tableName]->setDatabaseName(_dbName);
    _ptrSqlOptional[tableName]->setDriver(kcdz::SqlObject::SQLite);
    _ptrSqlOptional[tableName]->setTable(tableName);
}

template<class T>
Optional::TableOptional<T> *DBManager::getSqlOptional()
{
   Optional::TableOptional<T>* ptr=_ptrSqlOptional[typeid(T).name()].get();
    assert(ptr!=nullptr);
    return  ptr;
}
