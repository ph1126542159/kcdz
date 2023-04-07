#ifndef ROTATESTRATEGY_H
#define ROTATESTRATEGY_H
#include <QDateTime>
#include "LogFile.h"
namespace PH {
class  RotateStrategy{
public:
    RotateStrategy();

    virtual ~RotateStrategy();

    virtual bool mustRotate(LogFile* pFile) = 0;

private:
    RotateStrategy(const RotateStrategy&)=delete;
    RotateStrategy& operator = (const RotateStrategy&)=delete;
};


class RotateAtTimeStrategy : public RotateStrategy{
    //日志文件分割规则,按照给的时间进行分割[day,][hour]:minute
public:
    RotateAtTimeStrategy(const QString& rtime);
    ~RotateAtTimeStrategy(){
    }
    bool mustRotate(LogFile *pFile);
private:
    void getNextRollover();
    QDateTime  _threshold;
    int _day;
    int _hour;
    int _minute;
};


class RotateByIntervalStrategy :public RotateStrategy{
    //日志文件分割规则,按照创建时间 age
public:
    RotateByIntervalStrategy(const QDateTime& span);
    ~RotateByIntervalStrategy();
    bool mustRotate(LogFile* pFile);

private:
    QDateTime _span;
    QDateTime _lastRotate;
    static const std::string ROTATE_TEXT;
};
class  RotateBySizeStrategy: public RotateStrategy
{
public:
    RotateBySizeStrategy(quint64 size);
    ~RotateBySizeStrategy();
    bool mustRotate(LogFile* pFile);
private:
    quint64 _size;
};

}


#endif // ROTATESTRATEGY_H
