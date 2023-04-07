#include "RotateStrategy.h"
#include <QList>
#include "Core/Exception.h"
namespace PH {


RotateAtTimeStrategy::RotateAtTimeStrategy(const QString &rtime):
    _day(-1),
    _hour(-1),
    _minute(0)
{
    if(rtime.isEmpty()) throw PH::Exception("Rotation time must be specified");

    if(rtime.contains(",")&&rtime.indexOf(":")==rtime.size()-1) throw PH::Exception("Invalid rotation time specified.");

    QString tmTemp=rtime;

    QList<int> nNum;

    QStringList list=tmTemp.split(",");

    if(2==list.size()){
        nNum.push_back(list[0].toInt());
        tmTemp=list[1];
    }

    list=tmTemp.split(":");
    nNum.push_back(list[0].toInt());
    if(2==list.size()){
        nNum.push_back(list[1].toInt());
    }

    switch (nNum.count())
    {
    case 3: // day,hh:mm
    {
        _day=nNum[0];
        _hour=nNum[1];
        _minute=nNum[2];
        break;
    }
    case 2: // hh:mm
        _hour=nNum[0];
        _minute=nNum[1];
        break;
    case 1: // mm
        _minute=nNum[0];
        break;
    default:
        throw PH::Exception("Invalid rotation time specified.");
    }
    getNextRollover();
}

bool RotateAtTimeStrategy::mustRotate(LogFile *)
{
    if (QDateTime::currentDateTime() >= _threshold)
    {
        getNextRollover();
        return true;
    }
    return false;
}

void RotateAtTimeStrategy::getNextRollover()
{
    do
    {
        _threshold.setTime(_threshold.time().addSecs(1));
    }
    while (!(_threshold.time().minute() == _minute &&
             (-1 == _hour || _threshold.time().hour() == _hour) &&
             (-1 == _day  || _threshold.date().dayOfWeek() == _day)));

    _threshold=QDateTime(_threshold.date(),QTime(_threshold.time().hour(),_threshold.time().minute()));
}

RotateByIntervalStrategy::RotateByIntervalStrategy(const QDateTime &span):
    _span(span),
    _lastRotate(QDateTime::fromMSecsSinceEpoch(0))
{
    if(_span.toMSecsSinceEpoch()<=0) throw PH::Exception("time span must be greater than zero");
}

const std::string RotateByIntervalStrategy::ROTATE_TEXT("# Log file created/rotated ");

RotateByIntervalStrategy::~RotateByIntervalStrategy()
{
}

bool RotateByIntervalStrategy::mustRotate(LogFile *pFile)
{
    if(pFile->size()==0||_lastRotate.toMSecsSinceEpoch()==0){
        if(pFile->size()!=0){
            QString tag=pFile->readLine();
            if(tag.contains(ROTATE_TEXT.c_str())){
                tag=tag.mid(static_cast<int>(ROTATE_TEXT.length()),tag.length());
                _lastRotate=QDateTime::fromString(tag,"yyyy-MM-dd hh:mm:ss");
            }else _lastRotate=pFile->creationDate();
        }else{
            _lastRotate=QDateTime::currentDateTime();
            QString tag(ROTATE_TEXT.c_str());
            tag+=_lastRotate.toString("yyyy-MM-dd hh:mm:ss");
            pFile->write(tag);

        }
    }

    return _span<=QDateTime::fromMSecsSinceEpoch(QDateTime::currentDateTime().toMSecsSinceEpoch()-_lastRotate.toMSecsSinceEpoch());
}

RotateBySizeStrategy::RotateBySizeStrategy(quint64 size):
    _size(size)
{
    if(0==size) throw PH::Exception("size must be greater than zero");
}

RotateBySizeStrategy::~RotateBySizeStrategy()
{

}

bool RotateBySizeStrategy::mustRotate(LogFile *pFile)
{
    return pFile->size()>=_size;
}

RotateStrategy::RotateStrategy()
{

}

RotateStrategy::~RotateStrategy()
{

}



}
