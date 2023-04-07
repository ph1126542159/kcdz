#include "Logger.h"
#include "LoggingRegistry.h"
#include "Core/Exception.h"
namespace PH {
Logger::LoggerMapPtr Logger::_pLoggerMap;

std::mutex Logger::_mapMtx;

const std::string  Logger::ROOT;


Logger::Logger(const std::string& name, Channel::Ptr pChannel, int level): _name(name), _pChannel(pChannel), _level(level)
{
}


Logger::~Logger()
{
}


void Logger::setChannel(Channel::Ptr pChannel)
{
    _pChannel = pChannel;
}


Channel::Ptr Logger::getChannel() const
{
    return _pChannel;
}


void Logger::setLevel(int level)
{
    _level = level;
}


void Logger::setLevel(const std::string& level)
{
    setLevel(parseLevel(level));
}


void Logger::setProperty(const std::string& name, const std::string& value)
{
    if (name == "channel")
        setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
    else if (name == "level")
        setLevel(value);
    else
        Channel::setProperty(name, value);
}


void Logger::log(const Message& msg)
{
    if (_level >= msg.getPriority() && _pChannel.get())
    {
        _pChannel->log(msg);
    }
}


void Logger::log(const std::exception& exc)
{
    error(exc.what());
}


void Logger::log(const std::exception& exc, const char* file, int line)
{
    error(exc.what(), file, line);
}

void Logger::dump(const std::string &msg, const void *buffer, size_t length, Message::Priority prio)
{
    if (_level >= prio && _pChannel)
    {
        std::string text(msg);
        formatDump(text, buffer, length);
        _pChannel->log(Message(_name, text, prio));
    }
}

void Logger::setLevel(const std::string& name, int level)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    if (nullptr!=_pLoggerMap.get())
    {
        std::string::size_type len = name.length();
        for (auto& p: *_pLoggerMap)
        {
            if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
            {
                p.second->setLevel(level);
            }
        }
    }
}
void Logger::setChannel(const std::string& name, Channel::Ptr pChannel)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    if (nullptr!=_pLoggerMap.get())
    {
        std::string::size_type len = name.length();
        for (auto& p: *_pLoggerMap)
        {
            if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
            {
                p.second->setChannel(pChannel);
            }
        }
    }
}
void Logger::setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    if (nullptr!=_pLoggerMap.get())
    {
        std::string::size_type len = loggerName.length();
        for (auto& p: *_pLoggerMap)
        {
            if (len == 0 || (p.first.compare(0, len, loggerName) == 0 && (p.first.length() == len || p.first[len] == '.')))
            {
                p.second->setProperty(propertyName, value);
            }
        }
    }
}

Logger& Logger::get(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    return unsafeGet(name);
}


Logger& Logger::unsafeGet(const std::string& name)
{
    Ptr pLogger = find(name);
    if (!pLogger.get())
    {
        if (name == ROOT)
        {
            pLogger.reset(new Logger(name, nullptr, Message::PRIO_INFORMATION));
        }
        else
        {
            Logger& par = parent(name);
            pLogger.reset(new Logger(name, par.getChannel(), par.getLevel()));
        }
        add(pLogger);
    }
    return *pLogger;
}


Logger& Logger::create(const std::string& name, Channel::Ptr pChannel, int level)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    if (find(name)) throw PH::Exception("Exists Exception:"+name);
    Ptr pLogger =std::make_shared<Logger>(name, pChannel, level);
    add(pLogger);
    return *pLogger;
}


Logger& Logger::root()
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    return unsafeGet(ROOT);
}


Logger::Ptr Logger::has(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    return find(name);
}


void Logger::shutdown()
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    _pLoggerMap.reset();
}


Logger::Ptr Logger::find(const std::string& name)
{
    if (nullptr!=_pLoggerMap.get())
    {
        LoggerMap::iterator it = _pLoggerMap->find(name);
        if (it != _pLoggerMap->end()) return it->second;
    }
    return nullptr;
}


void Logger::destroy(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    if (nullptr!=_pLoggerMap.get())
    {
        LoggerMap::iterator it = _pLoggerMap->find(name);
        if (it != _pLoggerMap->end()) _pLoggerMap->erase(it);
    }
}


void Logger::names(std::vector<std::string>& names)
{
    std::lock_guard<std::mutex> lock(_mapMtx);

    names.clear();
    if (nullptr!=_pLoggerMap.get())
    {
        for (const auto& p: *_pLoggerMap)
        {
            names.push_back(p.first);
        }
    }
}


Logger& Logger::parent(const std::string& name)
{
    std::string::size_type pos = name.rfind('.');
    if (pos != std::string::npos)
    {
        std::string pname = name.substr(0, pos);
        Ptr pParent = find(pname);
        if (pParent.get())
            return *pParent;
        else
            return parent(pname);
    }
    else return unsafeGet(ROOT);
}


int Logger::parseLevel(const std::string& level)
{
    if (level.compare("none") == 0)
        return 0;
    else if (level.compare("fatal") == 0)
        return Message::PRIO_FATAL;
    else if (level.compare("critical") == 0)
        return Message::PRIO_CRITICAL;
    else if (level.compare("error") == 0)
        return Message::PRIO_ERROR;
    else if (level.compare("warning") == 0)
        return Message::PRIO_WARNING;
    else if (level.compare("notice") == 0)
        return Message::PRIO_NOTICE;
    else if (level.compare("information") == 0)
        return Message::PRIO_INFORMATION;
    else if (level.compare("debug") == 0)
        return Message::PRIO_DEBUG;
    else if (level.compare("trace") == 0)
        return Message::PRIO_TRACE;
    else
    {
        int numLevel=QString(level.c_str()).toInt();
        if (numLevel > 0 && numLevel < 9)
            return numLevel;
        else
            throw PH::Exception("Invalid Argument Exception Log level out of range "+level);
    }
}


void Logger::add(Ptr pLogger)
{
    if (!_pLoggerMap.get()) _pLoggerMap.reset(new LoggerMap);

    _pLoggerMap->insert(LoggerMap::value_type(pLogger->name(), pLogger));
}


void Logger::formatDump(std::string& message, const void* buffer, std::size_t length)
{
    const int BYTES_PER_LINE = 16;

    message.reserve(message.size() + length*6);
    if (!message.empty()) message.append("\n");
    unsigned char* base = (unsigned char*) buffer;
    int addr = 0;
    while (addr < length)
    {
        if (addr > 0) message.append("\n");
        message.append(formatHex<int,4>(addr));
        message.append("  ");
        int offset = 0;
        while (addr + offset < length && offset < BYTES_PER_LINE)
        {
            message.append(formatHex<unsigned char,2>(base[addr + offset]));
            message.append(offset == 7 ? "  " : " ");
            ++offset;
        }
        if (offset < 7) message.append(" ");
        while (offset < BYTES_PER_LINE) { message.append("   "); ++offset; }
        message.append(" ");
        offset = 0;
        while (addr + offset < length && offset < BYTES_PER_LINE)
        {
            unsigned char c = base[addr + offset];
            message += (c >= 32 && c < 127) ? (char) c : '.';
            ++offset;
        }
        addr += BYTES_PER_LINE;
    }
}

}
