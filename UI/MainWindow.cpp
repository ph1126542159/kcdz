#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QVBoxLayout>
#include <QMetaType>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QCloseEvent>


#define VERSION  "v1.35"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

