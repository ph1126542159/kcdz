#include "SerialPortSet.h"
#include "ui_SerialPortSet.h"
#include <QStringList>
#include <QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QDir>

SerialPortSet::SerialPortSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortSet)
{
    ui->setupUi(this);

    QStringList serialportInfo;
    foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()){
        serialportInfo<<info.portName();
    }

    ui->port1->addItems(serialportInfo);
    ui->preCheckport->addItems(serialportInfo);

}

SerialPortSet::~SerialPortSet()
{
    delete ui;
}

void SerialPortSet::on_ok_clicked()
{

    this->done(1);
}
