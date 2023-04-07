#include "ProductionSet.h"
#include "ui_ProductionSet.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

ProductionSet::ProductionSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductionSet)
{
    ui->setupUi(this);

}

ProductionSet::~ProductionSet()
{
    delete ui;
}
