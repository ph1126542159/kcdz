#include "TaskMgr.h"
#include "ui_TaskMgr.h"
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>

#include <QStandardItemModel>
#include <QTableView>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QInputDialog>

#include "DBManager.h"
#include "model/DET_PRODUCE.h"

#ifdef ENABLE_XLSS
#include <QtXlsx>
#endif
using namespace std;

enum TableHeadNo{
    T_ID  = 0 ,
    T_TASKID,
    T_OUTCODE   ,
    T_ENCIPHERRANRE,
    T_ASSIGNMENTNUM    ,
    T_ACCOMPLISHNUM       ,
    T_ABNORMALNUM   ,
    T_PRODUCETIME,
    T_LINE_LENGTH,
    T_LOTNUM,
    T_STAFF,
    T_STATUSCODE    ,
    T_PRODUCECOMMENT,
    T_PWD  ,
    T_DETTYPE   ,
    T_FACTORY    ,
    T_FEATURE,
    T_NUMBERINBOX,
    T_STARTBOX,
    T_T_STARTNO,
    T_PRODUCTIONLINEID,
    T_DETDATE,
    T_CODEPREFIX
};

TaskMgr::TaskMgr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskMgr)
{
    ui->setupUi(this);
//    QSqlTableModel* model= DBManager::getInstance()->getSqlOptional<TriModel::DET_PRODUCE>()->getSqlModel();
//    model->setSort(0,Qt::AscendingOrder);
//    ui->tableView_2->setModel(model);
//    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

TaskMgr::~TaskMgr()
{
    delete ui;
}

void TaskMgr::on_startProduce_released()
{
}

