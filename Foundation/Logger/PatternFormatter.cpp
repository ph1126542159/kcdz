#include "PatternFormatter.h"
#include <assert.h>
#include <QFileInfo>
namespace PH {
const std::string PatternFormatter::PROP_PATTERN = "pattern";
const std::string PatternFormatter::PROP_TIMES   = "times";
const std::string PatternFormatter::PROP_PRIORITY_NAMES = "priorityNames";


PatternFormatter::PatternFormatter()
{
    parsePriorityNames();
}


PatternFormatter::PatternFormatter(const std::string& format):
    _pattern(format)
{
    parsePriorityNames();
    parsePattern();
}


PatternFormatter::~PatternFormatter()
{
}


void PatternFormatter::format(const Message& msg, std::string& text)
{
    QDateTime timestamp = msg.getTime();
    QDateTime dateTime = timestamp;
    for (auto& pa:_patternActions)
    {
        text.append(pa.prepend);
        switch (pa.key)
        {
        case 's': text.append(msg.getSource()); break;
        case 't': text.append(msg.getText()); break;
        case 'l': text.append(std::to_string(static_cast<int>(msg.getPriority()))); break;
        case 'p': text.append(getPriorityName(static_cast<int>( msg.getPriority()))); break;
        case 'q': text += getPriorityName(static_cast<int>(msg.getPriority())).at(0); break;
        case 'P': text.append(QString("%1").arg(msg.getPid(),4,10,QChar('0')).toStdString()); break;
        case 'T': text.append(msg.getThread()); break;
        case 'I': text.append(QString("%1").arg(msg.getTid(),4,10,QChar('0')).toStdString()); break;
        case 'U': text.append(fillIn(msg.getSourceFile() ? msg.getSourceFile() : "",64,' ')); break;
        case 'O':text.append(fillIn(msg.getSourceFile()  ? QFileInfo(msg.getSourceFile()).fileName().toStdString().c_str() :"",24,' '));break;
        case 'u': text.append(QString("%1").arg(msg.getSourceLine(),4,10,QChar('0')).toStdString()); break;
        case 'w': text.append(QString("%1").arg(dateTime.date().dayOfWeek(),2,10,QChar('0')).toStdString()); break;
        case 'W': text.append(QString("%1").arg(dateTime.date().daysInMonth(),2,10,QChar('0')).toStdString()); break;
        case 'b': text.append(QString("%1").arg(dateTime.date().month(),2,10,QChar('0')).toStdString()); break;
        case 'B': text.append(QString("%1").arg(dateTime.date().month(),2,10,QChar('0')).toStdString()); break;
        case 'd': text.append(QString("%1").arg(dateTime.date().day(),2,10,QChar('0')).toStdString()); break;
        case 'e': text.append(QString("%1").arg(dateTime.date().day(),2,10,QChar('0')).toStdString()); break;
        case 'f': text.append(QString("%1").arg(dateTime.date().day(),2,10,QChar('0')).toStdString()); break;
        case 'm': text.append(QString("%1").arg(dateTime.date().month(),2,10,QChar('0')).toStdString()); break;
        case 'n': text.append(QString("%1").arg(dateTime.date().month(),2,10,QChar('0')).toStdString()); break;
        case 'o': text.append(QString("%1").arg(dateTime.date().month(),2,10,QChar('0')).toStdString()); break;
        case 'y': text.append(QString("%1").arg(dateTime.date().year(),4,10,QChar('0')).toStdString()); break;
        case 'Y': text.append(QString("%1").arg(dateTime.date().year(),4,10,QChar('0')).toStdString()); break;
        case 'H': text.append(QString("%1").arg(dateTime.time().hour(),2,10,QChar('0')).toStdString()); break;
        case 'h': text.append(QString("%1").arg(dateTime.time().hour(),2,10,QChar('0')).toStdString()); break;
        case 'M': text.append(QString("%1").arg(dateTime.time().minute(),2,10,QChar('0')).toStdString()); break;
        case 'S': text.append(QString("%1").arg(dateTime.time().second(),2,10,QChar('0')).toStdString()); break;
        case 'i': text.append(QString("%1").arg(dateTime.time().msec(),3,10,QChar('0')).toStdString()); break;
        case 'c': text.append(QString("%1").arg(dateTime.time().msec()/100,1,10,QChar('0')).toStdString()); break;
        case 'F': text.append(QString("%1").arg(dateTime.time().second()*1000 + dateTime.time().msec(),3,10,QChar('0')).toStdString()); break;
        case 'E': text.append(QString("%1").arg(msg.getTime().toTime_t(),8,10,QChar('0')).toStdString()); break;
        case 'v':
            if (pa.length > msg.getSource().length())
                text.append(msg.getSource()).append(pa.length - msg.getSource().length(), ' ');
            else if (pa.length && pa.length < msg.getSource().length())
                text.append(msg.getSource(), msg.getSource().length()-pa.length, pa.length);
            else
                text.append(msg.getSource());
            break;
        case 'x':
            try
        {
            text.append(msg[pa.property]);
        }
            catch (...)
        {
        }
            break;
        case 'L':
            break;
        }
    }
}

std::string PatternFormatter::fillIn(const std::string& str,int maxLen,char c){
    size_t len=str.length();
    std::string tmp=str;
    if(len<maxLen){
        for(size_t i =0 ; i < maxLen - len; i++)
            tmp+=c;
    }
    if(len>maxLen){
        for(size_t i =0 ; i < len-maxLen; i++)
            tmp.erase(tmp.begin());
    }
    return tmp;
}

void PatternFormatter::parsePattern()
{
    _patternActions.clear();
    std::string::const_iterator it  = _pattern.begin();
    std::string::const_iterator end = _pattern.end();
    PatternAction endAct;
    while (it != end)
    {
        if (*it == '%')
        {
            if (++it != end)
            {
                PatternAction act;
                act.prepend = endAct.prepend;
                endAct.prepend.clear();

                if (*it == '[')
                {
                    act.key = 'x';
                    ++it;
                    std::string prop;
                    while (it != end && *it != ']') prop += *it++;
                    if (it == end) --it;
                    act.property = prop;
                }
                else
                {
                    act.key = *it;
                    if ((it + 1) != end && *(it + 1) == '[')
                    {
                        it += 2;
                        std::string number;
                        while (it != end && *it != ']') number += *it++;
                        if (it == end) --it;
                        try
                        {
                            act.length = QString(number.c_str()).toUInt();
                        }
                        catch (...)
                        {
                        }
                    }
                }
                _patternActions.push_back(act);
                ++it;
            }
        }
        else
        {
            endAct.prepend += *it++;
        }
    }
    if (endAct.prepend.size())
    {
        _patternActions.push_back(endAct);
    }
}


void PatternFormatter::setProperty(const std::string& name, const std::string& value)
{
    if (name == PROP_PATTERN)
    {
        _pattern = value;
        parsePattern();
    }
    else if (name == PROP_PRIORITY_NAMES)
    {
        _priorityNames = value;
        parsePriorityNames();
    }else if(name == PROP_TIMES){

    }
    else
    {
        Formatter::setProperty(name, value);
    }
}


std::string PatternFormatter::getProperty(const std::string& name) const
{
    if (name == PROP_PATTERN)
        return _pattern;
    else if (name == PROP_PRIORITY_NAMES)
        return _priorityNames;
    else
        return Formatter::getProperty(name);
}


namespace
{
static std::string priorities[] =
{
    "     ",
    "Fatal",
    "Criti",
    "Error",
    "Warng",
    "Debug",
    "Notic",
    "Infor",
    "Tracer"
};
}


void PatternFormatter::parsePriorityNames()
{
    for (int i = 0; i <= 8; i++)
    {
        _priorities[i] = priorities[i];
    }
    if (!_priorityNames.empty())
    {
        QStringList strList=QString(_priorityNames.c_str()).split(",");
        QStringList temp;
        foreach (auto &it, strList) {
            if(it.contains(";")) temp<<it;
        }
        strList<<temp;
        if (strList.size() == 8)
        {
            for (int i = 1; i <= 8; i++)
            {
                _priorities[i] = strList.at(i - 1).toStdString();
            }
        }
        else throw PH::Exception("Syntax Exception priority Names property must specify a comma-separated list of 8 property names");
    }
}


const std::string& PatternFormatter::getPriorityName(int prio)
{
    assert (1 <= prio && prio <= 8);
    return priorities[prio];
}



}
