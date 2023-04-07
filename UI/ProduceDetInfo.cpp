#include "ProduceDetInfo.h"
#include "ui_ProduceDetInfo.h"

#include <QHeaderView>
#include <QFileDialog>
#include <QTextEdit>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <iostream>
#include <QInputDialog>

#ifdef ENABLE_XLSS
#include <QtXlsx>
#endif

using namespace std;

enum TableHeadNo{
    T_ID  = 0 ,
    T_OUTCODE   ,
    T_BARCODE   ,
    T_INCODE    ,
    T_PWD       ,
    T_DETTYPE   ,
    T_PRODUCETIME,
    T_CHANNEL    ,
    T_FAULTCODE  ,
    T_LINE_ID    ,
    T_FACTORY    ,
    T_FEATURE    ,
    T_LINE_LENGTH
};

ProduceDetInfo::ProduceDetInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProduceDetInfo)
{
    ui->setupUi(this);
}

ProduceDetInfo::~ProduceDetInfo()
{
    delete ui;
}
