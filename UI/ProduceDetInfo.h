#ifndef PRODUCEDETINFO_H
#define PRODUCEDETINFO_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <TaskMgr.h>

namespace Ui {
class ProduceDetInfo;
}

class ProduceDetInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ProduceDetInfo(QWidget *parent = nullptr);
    ~ProduceDetInfo();

private:
    Ui::ProduceDetInfo *ui;
};

#endif // PRODUCEDETINFO_H
