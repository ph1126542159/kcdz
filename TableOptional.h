#ifndef TABLEOPTIONAL_H
#define TABLEOPTIONAL_H
#include "Gui/SqlObject.h"
#include "Core/SharedPtr.h"
#include "qsqlrecord.h"
#include <QMetaProperty>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlIndex>

namespace Optional {
template<class T>
class TableOptional : public kcdz::SqlObject,public T{
public:
    explicit  TableOptional(QObject* parent=nullptr):
        kcdz::SqlObject(parent){
    }
    //    PH::SharedPtr<T> getRecord(int row){
    //        QSqlRecord record=_ptrModel->record(row);
    //        PH::SharedPtr<T>  ptr=new T;
    //        for(int i=0;i<ptr->metaObject()->propertyCount();i++){
    //            QMetaProperty proty=ptr->metaObject()->property(i);
    //            ptr->setProperty(proty.name(),record.value(proty.name()));
    //        }
    //        return ptr;
    //    }

    bool insertRecord( PH::SharedPtr<T> ptr){
        QSqlQuery query(getDataBase());
        return false;
    }

    //    bool removeRows(int startRow,int nCount=1){
    //        _ptrModel->database().transaction();
    //        if(_ptrModel->removeRows(startRow,nCount)&&_ptrModel->submitAll()){
    //            return _ptrModel->database().commit();
    //        }
    //        _ptrModel->database().rollback();
    //        _ptrModel->revertAll();
    //        qDebug() <<"insert Err:"<< _ptrModel->lastError();
    //        return false;
    //    }

    //    bool update(int row,PH::SharedPtr<T> ptr){
    //        QSqlRecord record = _ptrModel->record(row);
    //        for(int i=0;i<ptr->metaObject()->propertyCount();i++){
    //            QMetaProperty proty=ptr->metaObject()->property(i);
    //            record.setValue(proty.name(),ptr->property(proty.name()));
    //        }
    //        _ptrModel->database().transaction();
    //        if(_ptrModel->setRecord(row,record)&&_ptrModel->submitAll()){
    //            return _ptrModel->database().commit();
    //        }
    //        _ptrModel->database().rollback();
    //        _ptrModel->revertAll();
    //        qDebug() <<"insert Err:"<< _ptrModel->lastError();
    //        return false;
    //    }
private:
};
}
#endif // TABLEOPTIONAL_H
