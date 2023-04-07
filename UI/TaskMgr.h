#ifndef TASKMGR_H
#define TASKMGR_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>

namespace  Ui{
class TaskMgr;
}

class TaskMgr:public QWidget
{
     Q_OBJECT
public:
    explicit TaskMgr(QWidget *parent = nullptr);
    ~TaskMgr();


private:
    Ui::TaskMgr *ui;
};

#endif // TASKMGR_H
