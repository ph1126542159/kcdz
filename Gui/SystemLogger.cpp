#include "SystemLogger.h"
#include "Core/AutoPtr.h"
#include "Logger/FormattingChannel.h"
#include "Logger/ConsoleChannel.h"
#include "Logger/FileChannel.h"
#include "Logger/SplitterChannel.h"
#include "Logger/PatternFormatter.h"
#include "Logger/FormattingChannel.h"
#include "Logger/Logger.h"
#include "Logger/Message.h"
#include "Logger/AsyncChannel.h"



///Qt
#include <QSet>
namespace kcdz {

QSet<SystemLogger*> g_logger_objs;
QtMessageHandler kcdz_qt_logger_previousHandler = nullptr;
void kcdzQtLoggerFnc(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    for(SystemLogger *obj: g_logger_objs)
        obj->logMsg(type,context,msg);
}
class SystemLoggerPrivate{
public:
    QString _loggerName="System";
    QString _loggerPath="./log/System.log";
    QString _rotation="50 M";
    QString _archive="timestamp";
    QString _compress="true";
    QString _loggerFormat="%s [%p %l %Y-%m-%d %H:%M:%S.%i %O %u]:%t";
    QString _purgeAge="30 days ";
    int _loggerLevel=PH::Message::PRIO_TRACE;
    bool _enableConsle=false;
    bool _enableFile=false;


    void init(){
        if(!_channel.isNull()) return;

        _channel=new PH::SplitterChannel;
        if(_enableConsle){
            _channel->addChannel(new PH::ConsoleChannel);
        }

        if(_enableFile){
            PH::AutoPtr<PH::Formatter> formatter(new PH::PatternFormatter(_loggerFormat.toStdString()));
            PH::AutoPtr<PH::FileChannel> fileChannel=new PH::FileChannel(_loggerPath.toStdString());
            fileChannel->setProperty("rotation",_rotation.toStdString());
            fileChannel->setProperty("archive",_archive.toStdString());
            fileChannel->setProperty("compress",_compress.toStdString());
            fileChannel->setProperty("purgeAge",_purgeAge.toStdString());
            PH::AutoPtr<PH::Channel> formattingChannel(new PH::FormattingChannel(formatter, fileChannel));
            _pAsyncCh = new PH::AsyncChannel(formattingChannel);
            _pAsyncCh->open();
            _channel->addChannel(_pAsyncCh);
        }
		if (_channel.isNull()) return;
		PH::Logger::create(_loggerName.toStdString(), _channel, _loggerLevel);
    }

    void close(){
        if(!_pAsyncCh.isNull()) return;
        _pAsyncCh->close();
    }
private:
    PH::AsyncChannel::Ptr _pAsyncCh;
    PH::AutoPtr<PH::SplitterChannel> _channel;
};
void SystemLogger::logMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString text = msg.toLocal8Bit();
    switch (static_cast<int>(type)) {
    case QtDebugMsg:
        PH::Logger::get(p->_loggerName.toStdString()).debug(text,context.file,context.line);
        break;
    case QtWarningMsg:
        PH::Logger::get(p->_loggerName.toStdString()).warning(text,context.file,context.line);
        break;
    case QtCriticalMsg:
        PH::Logger::get(p->_loggerName.toStdString()).critical(text,context.file,context.line);
        break;
    case QtFatalMsg:
        PH::Logger::get(p->_loggerName.toStdString()).fatal(text,context.file,context.line);
        break;
    case QtInfoMsg:
        PH::Logger::get(p->_loggerName.toStdString()).trace(text,context.file,context.line);
        break;
    }
}
void SystemLogger::init()
{
    p->init();
    if(kcdz_qt_logger_previousHandler) return;
    kcdz_qt_logger_previousHandler=qInstallMessageHandler(kcdzQtLoggerFnc);
}

void SystemLogger::waitForFinished()
{
    p->close();
}

SystemLogger::SystemLogger(QObject *parent) : QObject(parent)
{
    p=new SystemLoggerPrivate;
	g_logger_objs.insert(this);
}

SystemLogger::~SystemLogger()
{
    g_logger_objs.remove(this);
    if( g_logger_objs.isEmpty() )
        qInstallMessageHandler(nullptr);
    delete p;
}

QString SystemLogger::loggerName()
{
    return p->_loggerName;
}

void SystemLogger::setLoggerName(const QString &name)
{
    p->_loggerName=name;
}
QString SystemLogger::loggerPath(){
    return p->_loggerPath;
}

void SystemLogger::setLoggerPath(const QString& value){
    p->_loggerPath=value;
}

QString SystemLogger::rotation(){
    return p->_rotation;
}

void SystemLogger::setRotation(const QString& value){
    p->_rotation=value;
}

QString SystemLogger::archive(){
    return p->_archive;
}

void SystemLogger::setArchive(const QString& value){
    p->_archive=value;
}

QString SystemLogger::compress(){
    return p->_compress;
}

void SystemLogger::setCompress(const QString& value){
    p->_compress=value;
}

QString SystemLogger::loggerFormat(){
    return p->_loggerFormat;
}

void SystemLogger::setLoggerFormat(const QString& value){
    p->_loggerFormat=value;
}

QString SystemLogger::purgeAge(){
    return p->_purgeAge;
}

void SystemLogger::setPurgeAge(const QString& value){
    p->_purgeAge=value;
}

int SystemLogger::loggerLevel(){
    return p->_loggerLevel;
}

void SystemLogger::setLoggerLevel(int value){
    p->_loggerLevel=value;
}

bool SystemLogger::enableConsle(){
    return p->_enableConsle;
}

void SystemLogger::setEnableConsle(bool value){
    p->_enableConsle=value;
}

bool SystemLogger::enableFile(){
    return p->_enableFile;
}

void SystemLogger::setEnableFile(bool value){
    p->_enableFile=value;
}
}

