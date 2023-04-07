#include "Message.h"
#include <QThread>
#include "Core/Exception.h"
namespace PH {


Message::Message():
    _prio(PRIO_FATAL),
    _time(QDateTime::currentDateTime().toLocalTime()),
    _tid(0),
    _pid(0),
    _file(nullptr),
    _line(0),
    _pMap(nullptr)
{
    init();
}


Message::Message(const std::string& source, const std::string& text, Priority prio):
    _source(source),
    _text(text),
    _prio(prio),
    _time(QDateTime::currentDateTime().toLocalTime()),
    _tid(0),
    _pid(0),
    _file(nullptr),
    _line(0),
    _pMap(nullptr)
{
    init();
}


Message::Message(const std::string& source, const std::string& text, Priority prio, const char* file, int line):
    _source(source),
    _text(text),
    _prio(prio),
    _time(QDateTime::currentDateTime().toLocalTime()),
    _tid(0),
    _pid(0),
    _file(file),
    _line(line),
    _pMap(nullptr)
{
    init();
}


Message::Message(const Message& msg):
    _source(msg._source),
    _text(msg._text),
    _prio(msg._prio),
    _time(msg._time),
    _tid(msg._tid),
    _thread(msg._thread),
    _pid(msg._pid),
    _file(msg._file),
    _line(msg._line)
{
    if (msg._pMap)
        _pMap = new StringMap(*msg._pMap);
    else
        _pMap = nullptr;
}


Message::Message(Message&& msg) noexcept:
    _source(std::move(msg._source)),
    _text(std::move(msg._text)),
    _prio(std::move(msg._prio)),
    _time(std::move(msg._time)),
    _tid(std::move(msg._tid)),
    _thread(std::move(msg._thread)),
    _pid(std::move(msg._pid)),
    _file(std::move(msg._file)),
    _line(std::move(msg._line))
{
    _pMap = msg._pMap;
    msg._pMap = nullptr;
}


Message::Message(const Message& msg, const std::string& text):
    _source(msg._source),
    _text(text),
    _prio(msg._prio),
    _time(msg._time),
    _tid(msg._tid),
    _thread(msg._thread),
    _pid(msg._pid),
    _file(msg._file),
    _line(msg._line)
{
    if (msg._pMap)
        _pMap = new StringMap(*msg._pMap);
    else
        _pMap = nullptr;
}


Message::~Message()
{
    delete _pMap;
}


void Message::init()
{
    QThread* pThread = QThread::currentThread();
    if (pThread)
    {
        _tid    =reinterpret_cast<long long>(pThread->currentThreadId());
        _thread = pThread->metaObject()->className();
    }
}


Message& Message::operator = (const Message& msg)
{
    if (&msg != this)
    {
        Message tmp(msg);
        swap(tmp);
    }
    return *this;
}


Message& Message::operator = (Message&& msg) noexcept
{
    _source = std::move(msg._source);
    _text = std::move(msg._text);
    _prio = std::move(msg._prio);
    _time = std::move(msg._time);
    _tid = std::move(msg._tid);
    _thread = std::move(msg._thread);
    _pid = std::move(msg._pid);
    _file = std::move(msg._file);
    _line = std::move(msg._line);
    delete _pMap;
    _pMap = msg._pMap;
    msg._pMap = nullptr;
    return *this;
}


void Message::swap(Message& msg)
{
    using std::swap;
    swap(_source, msg._source);
    swap(_text, msg._text);
    swap(_prio, msg._prio);
    swap(_time, msg._time);
    swap(_tid, msg._tid);
    swap(_thread, msg._thread);
    swap(_pid, msg._pid);
    swap(_file, msg._file);
    swap(_line, msg._line);
    swap(_pMap, msg._pMap);
}


void Message::setSource(const std::string& src)
{
    _source = src;
}


void Message::setText(const std::string& text)
{
    _text = text;
}


void Message::setPriority(Priority prio)
{
    _prio = prio;
}


void Message::setTime(const QDateTime& t)
{
    _time = t;
}


void Message::setThread(const std::string& thread)
{
    _thread = thread;
}


void Message::setTid(long tid)
{
    _tid = tid;
}


void Message::setPid(long pid)
{
    _pid = pid;
}


void Message::setSourceFile(const char* file)
{
    _file = file;
}


void Message::setSourceLine(int line)
{
    _line = line;
}


bool Message::has(const std::string& param) const
{
    return _pMap && (_pMap->find(param) != _pMap->end());
}


const std::string& Message::get(const std::string& param) const
{
    if (_pMap)
    {
        StringMap::const_iterator it = _pMap->find(param);
        if (it != _pMap->end())
            return it->second;
    }

    throw PH::Exception("Not Found Exception");
}


const std::string& Message::get(const std::string& param, const std::string& defaultValue) const
{
    if (_pMap)
    {
        StringMap::const_iterator it = _pMap->find(param);
        if (it != _pMap->end())
            return it->second;
    }

    return defaultValue;
}


void Message::set(const std::string& param, const std::string& value)
{
    if (!_pMap)
        _pMap = new StringMap;

    std::pair<StringMap::iterator, bool> result =
            _pMap->insert(std::make_pair(param, value));
    if (!result.second)
    {
        result.first->second = value;
    }
}


const std::string& Message::operator [] (const std::string& param) const
{
    if (_pMap)
        return (*_pMap)[param];
    else
        throw PH::Exception("Not Found Exception");
}


std::string& Message::operator [] (const std::string& param)
{
    if (!_pMap)
        _pMap = new StringMap;
    return (*_pMap)[param];
}


}
