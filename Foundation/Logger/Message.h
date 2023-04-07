#ifndef MESSAGE_H
#define MESSAGE_H
#include <map>
#include <QDateTime>
#include <memory>

namespace PH {


class   Message
{
public:
    enum Priority
    {
        PRIO_FATAL = 1,
        PRIO_CRITICAL,
        PRIO_ERROR,
        PRIO_WARNING,
        PRIO_DEBUG,
        PRIO_NOTICE,
        PRIO_INFORMATION,
        PRIO_TRACE
    };

    using Ptr=std::shared_ptr<Message>;

    Message();


    Message(const std::string& source, const std::string& text, Priority prio);


    Message(const std::string& source, const std::string& text, Priority prio, const char* file, int line);


    Message(const Message& msg);


    Message(Message&& msg) noexcept;


    Message(const Message& msg, const std::string& text);

    ~Message();


    Message& operator = (const Message& msg);


    Message& operator = (Message&& msg) noexcept;


    void swap(Message& msg);


    void setSource(const std::string& src);


    const std::string& getSource() const;


    void setText(const std::string& text);


    const std::string& getText() const;


    void setPriority(Priority prio);


    Priority getPriority() const;


    void setTime(const QDateTime& time);


    const QDateTime& getTime() const;


    void setThread(const std::string& thread);


    const std::string& getThread() const;


    void setTid(long pid);


    long getTid() const;

    void setPid(long pid);

    long getPid() const;

    void setSourceFile(const char* file);

    const char* getSourceFile() const;

    void setSourceLine(int line);

    int getSourceLine() const;


    bool has(const std::string& param) const;

    const std::string& get(const std::string& param) const;


    const std::string& get(const std::string& param, const std::string& defaultValue) const;


    void set(const std::string& param, const std::string& value);


    const std::string& operator [] (const std::string& param) const;


    std::string& operator [] (const std::string& param);

protected:
    void init();
    typedef std::map<std::string, std::string> StringMap;

private:
    std::string _source;
    std::string _text;
    Priority    _prio;
    QDateTime   _time;
    long long   _tid;
    std::string _thread;
    long        _pid;
    const char* _file;
    int         _line;
    StringMap*  _pMap;
};


//
// inlines
//
inline const std::string& Message::getSource() const
{
    return _source;
}


inline const std::string& Message::getText() const
{
    return _text;
}


inline Message::Priority Message::getPriority() const
{
    return _prio;
}


inline const QDateTime& Message::getTime() const
{
    return _time;
}


inline const std::string& Message::getThread() const
{
    return _thread;
}


inline long Message::getTid() const
{
    return _tid;
}


inline long Message::getPid() const
{
    return _pid;
}


inline const char* Message::getSourceFile() const
{
    return _file;
}


inline int Message::getSourceLine() const
{
    return _line;
}


inline void swap(Message& m1, Message& m2)
{
    m1.swap(m2);
}


}

#endif // MESSAGE_H
