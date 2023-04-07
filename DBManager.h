#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include "Core/SharedPtr.h"
#include "Core/Singleholder.h"
#include <QHash>
#include "Gui/SqlObject.h"

#include "TableOptional.h"

class DBManager : public QObject,public PH::SingleHolder<DBManager>
{
    Q_OBJECT
public:
    bool init();
    template<class T>
    Optional::TableOptional<T>* getSqlOptional();
private:
    template<class T>
    void insertSqlOptional();
    friend class PH::SingleHolder<DBManager>;
    explicit DBManager(QObject *parent = nullptr);
    QString _dbName;
    QHash<QString,PH::SharedPtr<kcdz::SqlObject>> _ptrSqlOptional;
};

#endif // DBMANAGER_H
