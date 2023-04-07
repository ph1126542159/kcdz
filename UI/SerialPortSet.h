#ifndef SERIALPORTSET_H
#define SERIALPORTSET_H

#include <QDialog>

namespace Ui {
class SerialPortSet;
}

class SerialPortSet : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortSet(QWidget *parent = nullptr);
    ~SerialPortSet();

private slots:
    void on_ok_clicked();

private:
    Ui::SerialPortSet *ui;

    QString m_port1;
    QString m_port2;
};

#endif // SERIALPORTSET_H
