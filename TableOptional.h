#ifndef TABLEOPTIONAL_H
#define TABLEOPTIONAL_H
#include "Gui/SqlObject.h"
#include <QSqlTableModel>
namespace Table {
class DET_PRODUCE : public QObject{
    Q_OBJECT
public:
    explicit DET_PRODUCE(QObject *parent = nullptr);
};
}
namespace Optional {
template<class T>
class TableOptional : public kcdz::SqlObject{
public:
    TableOptional(){
        _model.setTable(typeid(T).name());
    }

private:
    QSqlTableModel _model;
};

}

#endif // TABLEOPTIONAL_H
