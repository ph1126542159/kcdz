﻿#include "LogWidget.h"
#include "ui_LogWidget.h"



LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);




}

LogWidget::~LogWidget()
{
    delete ui;
}
