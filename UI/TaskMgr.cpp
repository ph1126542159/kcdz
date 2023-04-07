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

}

TaskMgr::~TaskMgr()
{
     delete ui;
}
