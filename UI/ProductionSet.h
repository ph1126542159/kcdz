#ifndef PRODUCTIONSET_H
#define PRODUCTIONSET_H

#include <QDialog>

struct ProduceConfigInfo {
    QString productDate;
    int lineLength; //脚线长度
    int produceLineID;
    int numInBox;  //盒内数量
    int startBox; //起始盒号
    int startNo;  //起始序号
    int produceNum;  //生产数量
    uchar feature;
};

namespace Ui {
class ProductionSet;
}

class ProductionSet : public QDialog
{
    Q_OBJECT

public:
    explicit ProductionSet(QWidget *parent = nullptr);
    ~ProductionSet();


private:
    Ui::ProductionSet *ui;
};

#endif // PRODUCTIONSET_H
