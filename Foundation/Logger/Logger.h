#ifndef LOGGER_H
#define LOGGER_H
#include "Channel.h"
#include "Message.h"
#include <map>
#include <vector>
#include <memory>
#include <mutex>
#include <string>


namespace PH {
class   Logger: public Channel{
public:

    using Ptr = std::shared_ptr<Logger>;
    Logger(const std::string& name, Channel::Ptr pChannel, int level);

    ~Logger();

    const std::string& name() const;

    void setChannel(Channel::Ptr pChannel);

    Channel::Ptr getChannel() const;

    void setLevel(int level);

    int getLevel() const;

    void setLevel(const std::string& level);
    ///   - none (turns off logging)
    ///   - fatal
    ///   - critical
    ///   - error
    ///   - warning
    ///   - notice
    ///   - information
    ///   - debug
    ///   - trace

    void setProperty(const std::string& name, const std::string& value);

    void log(const Message& msg);

    void log(const std::exception& exc);

    void log(const std::exception& exc, const char* file, int line);

    void fatal(const QString& msg);

    void fatal(const QString& msg, const char* file, int line);

    void critical(const QString& msg);

    void critical(const QString& msg, const char* file, int line);

    void error(const QString& msg);

    void error(const QString& msg, const char* file, int line);

    void warning(const QString& msg);

    void warning(const QString& msg, const char* file, int line);

    void notice(const QString& msg);

    void notice(const QString& msg, const char* file, int line);

    void information(const QString& msg);

    void information(const QString& msg, const char* file, int line);

    void debug(const QString& msg);

    void debug(const QString& msg, const char* file, int line);

    void trace(const QString& msg);

    void trace(const QString& msg, const char* file, int line);

    void dump(const std::string& msg, const void* buffer, std::size_t length, Message::Priority prio = Message::PRIO_DEBUG);

    bool is(int level) const;

    bool fatal() const;

    bool critical() const;

    bool error() const;

    bool warning() const;

    bool notice() const;

    bool information() const;

    bool debug() const;

    bool trace() const;

    static void setLevel(const std::string& name, int level);

    static void setChannel(const std::string& name, Channel::Ptr pChannel);

    static void setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value);

    static Logger& get(const std::string& name);

    static Logger& unsafeGet(const std::string& name);

    static Logger& create(const std::string& name, Channel::Ptr pChannel, int level = Message::PRIO_INFORMATION);

    static Logger& root();

    static Ptr has(const std::string& name);

    static void destroy(const std::string& name);

    static void shutdown();

    static void names(std::vector<std::string>& names);

    static int parseLevel(const std::string& level);

    static void formatDump(std::string& message, const void* buffer, std::size_t length);

    template<typename T,unsigned short N>
    static std::string formatHex(T num){
        char tmp[N];
        memcpy(tmp,&num,sizeof(T)<N ? sizeof(T):N);
        QByteArray bt(tmp,N);
        bt=bt.toHex();
        QString result;
        for(int i = 0; i < bt.length()/2;i++) result+=bt.mid(i*2,2)+" ";
        return result.toStdString();
    }
    ///   - none (turns off logging)
    ///   - fatal
    ///   - critical
    ///   - error
    ///   - warning
    ///   - notice
    ///   - information
    ///   - debug
    ///   - trace
    ///
    static const std::string ROOT;
protected:
    typedef std::map<std::string, Ptr> LoggerMap;

    void log(const QString& text, Message::Priority prio);

    void log(const QString& text, Message::Priority prio, const char* file, int line);

    static Logger& parent(const std::string& name);
    static void add(Ptr pLogger);
    static Ptr find(const std::string& name);

private:
    typedef std::unique_ptr<LoggerMap> LoggerMapPtr;

    Logger();
    Logger(const Logger&);
    Logger& operator = (const Logger&);

    std::string _name;
    Channel::Ptr _pChannel;
    int         _level;

    static LoggerMapPtr _pLoggerMap;

    static std::mutex      _mapMtx;
};

//
// inlines
//
inline const std::string& Logger::name() const
{
    return _name;
}


inline int Logger::getLevel() const
{
    return _level;
}


inline void Logger::log(const QString& text, Message::Priority prio)
{
    if (_level >= prio && _pChannel.get())
    {
        _pChannel->log(Message(_name, text.toStdString(), prio));
    }
}


inline void Logger::log(const QString& text, Message::Priority prio, const char* file, int line)
{
    if (_level >= prio && _pChannel.get())
    {
        _pChannel->log(Message(_name, text.toStdString(), prio, file, line));
    }
}


inline void Logger::fatal(const QString& msg)
{
    log(msg, Message::PRIO_FATAL);
}


inline void Logger::fatal(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_FATAL, file, line);
}



inline void Logger::critical(const QString& msg)
{
    log(msg, Message::PRIO_CRITICAL);
}


inline void Logger::critical(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_CRITICAL, file, line);
}


inline void Logger::error(const QString& msg)
{
    log(msg, Message::PRIO_ERROR);
}


inline void Logger::error(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_ERROR, file, line);
}


inline void Logger::warning(const QString& msg)
{
    log(msg, Message::PRIO_WARNING);
}


inline void Logger::warning(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_WARNING, file, line);
}


inline void Logger::notice(const QString& msg)
{
    log(msg, Message::PRIO_NOTICE);
}


inline void Logger::notice(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_NOTICE, file, line);
}


inline void Logger::information(const QString& msg)
{
    log(msg, Message::PRIO_INFORMATION);
}


inline void Logger::information(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_INFORMATION, file, line);
}


inline void Logger::debug(const QString& msg)
{
    log(msg, Message::PRIO_DEBUG);
}


inline void Logger::debug(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_DEBUG, file, line);
}


inline void Logger::trace(const QString& msg)
{
    log(msg, Message::PRIO_TRACE);
}


inline void Logger::trace(const QString& msg, const char* file, int line)
{
    log(msg, Message::PRIO_TRACE, file, line);
}


inline bool Logger::is(int level) const
{
    return _level >= level;
}


inline bool Logger::fatal() const
{
    return _level >= Message::PRIO_FATAL;
}


inline bool Logger::critical() const
{
    return _level >= Message::PRIO_CRITICAL;
}


inline bool Logger::error() const
{
    return _level >= Message::PRIO_ERROR;
}


inline bool Logger::warning() const
{
    return _level >= Message::PRIO_WARNING;
}


inline bool Logger::notice() const
{
    return _level >= Message::PRIO_NOTICE;
}


inline bool Logger::information() const
{
    return _level >= Message::PRIO_INFORMATION;
}


inline bool Logger::debug() const
{
    return _level >= Message::PRIO_DEBUG;
}


inline bool Logger::trace() const
{
    return _level >= Message::PRIO_TRACE;
}
}

#endif // LOGGER_H
