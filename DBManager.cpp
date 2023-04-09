#include "DBManager.h"
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QMetaProperty>
#include "model/productDetData.h"
#include "model/DET_PRODUCE.h"
#include <QThreadPool>

TestWrite::TestWrite()
{

}

void TestWrite::run()
{
    PH::SharedPtr<TriModel::DET_PRODUCE> ptr=DBManager::getInstance()->getSqlOptional<TriModel::DET_PRODUCE>();
    ptr->setID(3);
    ptr->setTASKID("1111111111111");
    ptr->setCODEPREFIX("222222222222222");
    ptr->setOUTCODE("SSSSSSSS");
    ptr->setBARCODE("EEEEEEEE");
    ptr->setINCODE("DDDDDDDDDDD");
    ptr->setPWD("FFFFFFFFFFFF");
    ptr->setCHANNEL(1);
    ptr->setDET_TYPE(23);
    ptr->setSTATUS_CODE(23);
    ptr->setPRODUCTION_LINE_ID(44);
    ptr->setLINE_LENGTH(34);
    ptr->setFACTORY(77);
    ptr->setFEATURE("hhhhhh");
    ptr->setDET_DATE("876876");
    ptr->setNO(666);
    ptr->setPRODUCE_TIME("8888888888");
    while (true) {
        emit testInsert();
        QThread::msleep(2000);
        return;
    }
}

TestWrite* g_test=new TestWrite;

DBManager::DBManager(QObject *parent)
    : QObject{parent}
{
    QObject::connect(g_test,&TestWrite::testInsert,this,&DBManager::testInsertDlot,Qt::QueuedConnection);
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

    insertSqlOptional<TriModel::DET_PRODUCE>();
    // insertSqlOptional<TriModel::productDetData>();

    qInfo()<<"open db succed,db name:"<<_dbName;

    //*************************************test**************************************//
    QThreadPool::globalInstance()->start(g_test);
    return true;
}

void DBManager::testInsertDlot()
{
    std::unique_lock<std::mutex> lock(std::mutex);
    PH::SharedPtr<TriModel::DET_PRODUCE> ptr= getSqlOptional<TriModel::DET_PRODUCE>();
//    ptr->deleteQuery("ID=2");
    ptr->update("ID=3");
    qDebug()<<ptr->lastError();
//    ptr->insert();
//    QVariantList lsit;
//    ptr->getNextPage(lsit);
//    qDebug()<<lsit.size()<<ptr->getPageIndex();
}
template<class T>
void DBManager::insertSqlOptional()
{
    PH::SharedPtr<T> ptr=new T();
    ptr->initSql(_dbName);
    _ptrSqlOptional[typeid(T).name()]=ptr;
}
