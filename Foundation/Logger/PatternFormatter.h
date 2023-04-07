#ifndef PATTERNFORMATTER_H
#define PATTERNFORMATTER_H
#include "Formatter.h"
#include "Message.h"
#include <vector>

namespace PH {
class   PatternFormatter: public Formatter{
    /// * %s -消息源

    /// * %t -消息文本

    /// * %l -消息优先级(1 ..7）

    /// * %p -消息优先级(致命，严重，错误，警告，通知，信息，调试，跟踪)

    /// * %q -缩写消息优先级(F, C, E, W, N, I, D, T)

    /// * %P -消息进程标识符

    /// * %T -消息线程名

    /// * %I -消息线程标识符(数字)

    /// * %U -消息源文件路径(空字符串，如果没有设置)

    /// * %u -消息源行号(如果没有设置则为0)

    /// * %w -消息日期/时间缩写星期(星期一，星期二，…)

    /// * %W -消息日期/时间满工作日(星期一，星期二，…)

    /// * %b -消息日期/时间缩写月(Jan, Feb，…)

    /// * %B - message date/time full month (January, February，…)

    /// * %d -消息日期/时间零填充日的月(01 ..31）

    /// * %e - message date/time day of month (1 ..31）

    /// * %f - message date/time - spacing - padding day of month (1 ..31）

    /// * %m -消息日期/时间零填充月(01 ..12）

    /// * %n - message date/time month (1 ..12）

    /// * %o -消息日期/时间空格填充月(1 ..12）

    /// * %y - message date/time year (70)

    /// * %Y - message date/time year with century (1970)

    /// * %H -消息日期/时间小时(00 ..23）

    /// * %h -消息日期/时间小时(00 ..12）

    /// * %a - message日期/时间am/pm

    /// * %A -消息日期/时间AM/PM

    /// * %M -消息日期/时间分钟(00 ..59）

    /// * %S -消息日期/时间秒(00 ..59）

    /// * %i -消息日期/时间毫秒(000 ..999)

    /// * %c -消息日期/时间厘秒(0 ..9）

    /// * %F -消息日期/时间小数秒/微秒(000000 - 999999)

    /// * %z - ISO 8601格式的时区差异(z或+NN.NN)

    /// * %Z - RFC格式的时区差异(GMT或+NNNN)

    /// * %L -将时间转换为本地时间(必须在任何日期/时间指定符之前指定;本身不输出任何东西)

    /// * %E - epoch time (UTC, seconds since midnight, January 1, 1970)

    /// * %v[width] -消息源(%s)，但文本长度被填充/裁剪为'width'

    /// * %[name] -具有给定名称的消息参数的值

    /// * %% - %符号
    ///
public:
    using Ptr=std::shared_ptr<PatternFormatter>;

    PatternFormatter();

    PatternFormatter(const std::string& format);

    ~PatternFormatter();

    void format(const Message& msg, std::string& text);

    void setProperty(const std::string& name, const std::string& value);

    std::string getProperty(const std::string& name) const;

    static const std::string PROP_PATTERN;
    static const std::string PROP_TIMES;
    static const std::string PROP_PRIORITY_NAMES;
protected:
    const std::string& getPriorityName(int);
private:
    struct PatternAction
    {
        PatternAction(): key(0), length(0)
        {
        }

        char key;
        std::size_t length;
        std::string property;
        std::string prepend;
    };

    void parsePattern();

    void parsePriorityNames();

    std::string fillIn(const std::string& str,int maxLen,char c);

    std::vector<PatternAction> _patternActions;
    std::string _pattern;
    std::string _priorityNames;
    std::string _priorities[9];
};
}

#endif // PATTERNFORMATTER_H
