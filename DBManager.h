#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include "Core/SharedPtr.h"
#include "Core/Singleholder.h"
#include <QHash>
#include "Gui/SqlObject.h"
#include <mutex>

#include <QRunnable>
class TestWrite :public QObject, public QRunnable{
    Q_OBJECT
public:
    TestWrite();
    void run()override;
Q_SIGNALS:
    void testInsert();
};

class DBManager : public QObject,public PH::SingleHolder<DBManager>
{
    Q_OBJECT
public:
    bool init();
    template<class T>
    PH::SharedPtr<T> getSqlOptional();
private Q_SLOTS:
    void testInsertDlot();
private:
    template<class T>
    void insertSqlOptional();
    friend class PH::SingleHolder<DBManager>;
    explicit DBManager(QObject *parent = nullptr);
    QString _dbName;
    QHash<QString,PH::SharedPtr<kcdz::SqlObject>> _ptrSqlOptional;
};
template<class T>
PH::SharedPtr<T>DBManager::getSqlOptional()
{
    std::unique_lock<std::mutex> lock(std::mutex);
    PH::SharedPtr<T> ptr=_ptrSqlOptional[typeid(T).name()].cast<T>();
    return  ptr;
}

#endif // DBMANAGER_H
