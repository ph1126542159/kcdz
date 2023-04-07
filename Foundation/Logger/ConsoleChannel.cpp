#include "ConsoleChannel.h"
#include <iostream>
#include "Message.h"


namespace PH {


std::mutex ConsoleChannel::_mutex;


ConsoleChannel::ConsoleChannel(): _str(std::clog)
{
}


ConsoleChannel::ConsoleChannel(std::ostream& str): _str(str)
{
}


ConsoleChannel::~ConsoleChannel()
{
}


void ConsoleChannel::log(const Message& msg)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _str << msg.getText() << std::endl;
}


std::mutex ColorConsoleChannel::_mutex;
const std::string ColorConsoleChannel::CSI("\033[");


ColorConsoleChannel::ColorConsoleChannel():
    _str(std::clog),
    _enableColors(true)
{
    initColors();
}


ColorConsoleChannel::ColorConsoleChannel(std::ostream& str):
    _str(str),
    _enableColors(true)
{
    initColors();
}


ColorConsoleChannel::~ColorConsoleChannel()
{
}


void ColorConsoleChannel::log(const Message& msg)
{
    std::lock_guard<std::mutex> _lock(_mutex);

    if (_enableColors)
    {
        int color = _colors[msg.getPriority()];
        if (color & 0x100)
        {
            _str << CSI << "1m";
        }
        color &= 0xff;
        _str << CSI << color << "m";
    }

    _str << msg.getText();

    if (_enableColors)
    {
        _str << CSI << "0m";
    }

    _str << std::endl;
}


void ColorConsoleChannel::setProperty(const std::string& name, const std::string& value)
{
    if (name == "enableColors")
    {
        _enableColors = value.compare("true") == 0;
    }
    else if (name == "traceColor")
    {
        _colors[Message::PRIO_TRACE] = parseColor(value);
    }
    else if (name == "debugColor")
    {
        _colors[Message::PRIO_DEBUG] = parseColor(value);
    }
    else if (name == "informationColor")
    {
        _colors[Message::PRIO_INFORMATION] = parseColor(value);
    }
    else if (name == "noticeColor")
    {
        _colors[Message::PRIO_NOTICE] = parseColor(value);
    }
    else if (name == "warningColor")
    {
        _colors[Message::PRIO_WARNING] = parseColor(value);
    }
    else if (name == "errorColor")
    {
        _colors[Message::PRIO_ERROR] = parseColor(value);
    }
    else if (name == "criticalColor")
    {
        _colors[Message::PRIO_CRITICAL] = parseColor(value);
    }
    else if (name == "fatalColor")
    {
        _colors[Message::PRIO_FATAL] = parseColor(value);
    }
    else
    {
        Channel::setProperty(name, value);
    }
}


std::string ColorConsoleChannel::getProperty(const std::string& name) const
{
    if (name == "enableColors")
    {
        return _enableColors ? "true" : "false";
    }
    else if (name == "traceColor")
    {
        return formatColor(_colors[Message::PRIO_TRACE]);
    }
    else if (name == "debugColor")
    {
        return formatColor(_colors[Message::PRIO_DEBUG]);
    }
    else if (name == "informationColor")
    {
        return formatColor(_colors[Message::PRIO_INFORMATION]);
    }
    else if (name == "noticeColor")
    {
        return formatColor(_colors[Message::PRIO_NOTICE]);
    }
    else if (name == "warningColor")
    {
        return formatColor(_colors[Message::PRIO_WARNING]);
    }
    else if (name == "errorColor")
    {
        return formatColor(_colors[Message::PRIO_ERROR]);
    }
    else if (name == "criticalColor")
    {
        return formatColor(_colors[Message::PRIO_CRITICAL]);
    }
    else if (name == "fatalColor")
    {
        return formatColor(_colors[Message::PRIO_FATAL]);
    }
    else
    {
        return Channel::getProperty(name);
    }
}


ColorConsoleChannel::Color ColorConsoleChannel::parseColor(const std::string& color) const
{
    if (color.compare("default") == 0)
        return CC_DEFAULT;
    else if (color.compare( "black") == 0)
        return CC_BLACK;
    else if (color.compare( "red") == 0)
        return CC_RED;
    else if (color.compare( "green") == 0)
        return CC_GREEN;
    else if (color.compare( "brown") == 0)
        return CC_BROWN;
    else if (color.compare( "blue") == 0)
        return CC_BLUE;
    else if (color.compare( "magenta") == 0)
        return CC_MAGENTA;
    else if (color.compare( "cyan") == 0)
        return CC_CYAN;
    else if (color.compare( "gray") == 0)
        return CC_GRAY;
    else if (color.compare( "darkGray") == 0)
        return CC_DARKGRAY;
    else if (color.compare( "lightRed") == 0)
        return CC_LIGHTRED;
    else if (color.compare( "lightGreen") == 0)
        return CC_LIGHTGREEN;
    else if (color.compare( "yellow") == 0)
        return CC_YELLOW;
    else if (color.compare( "lightBlue") == 0)
        return CC_LIGHTBLUE;
    else if (color.compare( "lightMagenta") == 0)
        return CC_LIGHTMAGENTA;
    else if (color.compare( "lightCyan") == 0)
        return CC_LIGHTCYAN;
    else if (color.compare( "white") == 0)
        return CC_WHITE;

     throw PH::Exception("Invalid color value"+color);
}


std::string ColorConsoleChannel::formatColor(Color color) const
{
    switch (color)
    {
    case CC_DEFAULT:      return "default";
    case CC_BLACK:        return "black";
    case CC_RED:          return "red";
    case CC_GREEN:        return "green";
    case CC_BROWN:        return "brown";
    case CC_BLUE:         return "blue";
    case CC_MAGENTA:      return "magenta";
    case CC_CYAN:         return "cyan";
    case CC_GRAY:         return "gray";
    case CC_DARKGRAY:     return "darkGray";
    case CC_LIGHTRED:     return "lightRed";
    case CC_LIGHTGREEN:   return "lightGreen";
    case CC_YELLOW:       return "yellow";
    case CC_LIGHTBLUE:    return "lightBlue";
    case CC_LIGHTMAGENTA: return "lightMagenta";
    case CC_LIGHTCYAN:    return "lightCyan";
    case CC_WHITE:        return "white";
    }

    return "invalid";
}


void ColorConsoleChannel::initColors()
{
    _colors[0] = CC_DEFAULT; // unused
    _colors[Message::PRIO_FATAL]       = CC_LIGHTRED;
    _colors[Message::PRIO_CRITICAL]    = CC_LIGHTRED;
    _colors[Message::PRIO_ERROR]       = CC_LIGHTRED;
    _colors[Message::PRIO_WARNING]     = CC_YELLOW;
    _colors[Message::PRIO_NOTICE]      = CC_DEFAULT;
    _colors[Message::PRIO_INFORMATION] = CC_DEFAULT;
    _colors[Message::PRIO_DEBUG]       = CC_GRAY;
    _colors[Message::PRIO_TRACE]       = CC_GRAY;
}


}
