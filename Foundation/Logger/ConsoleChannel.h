#ifndef CONSOLECHANNEL_H
#define CONSOLECHANNEL_H
#include "Channel.h"
#include <ostream>
#include <mutex>

namespace PH {


class   ConsoleChannel: public Channel
{
public:
    using Ptr = std::shared_ptr<ConsoleChannel>;

    ConsoleChannel();

    ~ConsoleChannel();

    ConsoleChannel(std::ostream& str);


    void log(const Message& msg);

protected:


private:
    std::ostream& _str;
    static std::mutex _mutex;
};


class  ColorConsoleChannel: public Channel
{
public:
    ColorConsoleChannel();

    ColorConsoleChannel(std::ostream& str);

    void log(const Message& msg);

    void setProperty(const std::string& name, const std::string& value);


    std::string getProperty(const std::string& name) const;

protected:
    enum Color
    {
        CC_DEFAULT      = 0x0027,
        CC_BLACK        = 0x001e,
        CC_RED          = 0x001f,
        CC_GREEN        = 0x0020,
        CC_BROWN        = 0x0021,
        CC_BLUE         = 0x0022,
        CC_MAGENTA      = 0x0023,
        CC_CYAN         = 0x0024,
        CC_GRAY         = 0x0025,
        CC_DARKGRAY     = 0x011e,
        CC_LIGHTRED     = 0x011f,
        CC_LIGHTGREEN   = 0x0120,
        CC_YELLOW       = 0x0121,
        CC_LIGHTBLUE    = 0x0122,
        CC_LIGHTMAGENTA = 0x0123,
        CC_LIGHTCYAN    = 0x0124,
        CC_WHITE        = 0x0125
    };

    ~ColorConsoleChannel();
    Color parseColor(const std::string& color) const;
    std::string formatColor(Color color) const;
    void initColors();

private:
    std::ostream& _str;
    bool _enableColors;
    Color _colors[9];
    static std::mutex _mutex;
    static const std::string CSI;
};


} // namespace PH
#endif // CONSOLECHANNEL_H
