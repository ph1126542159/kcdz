#include <QDir>
#include "MainWindow.h"
#include "DBManager.h"
#include <QDebug>
#include "Core/SharedPtr.h"
#include "Gui/SystemLogger.h"
#include "GlobalSetting.h"
#include "Gui/Application.h"
#include <QMessageBox>
///当前切换到 vs 有效
#define ENABLE_CRASH

#if defined(WIN32) && defined(ENABLE_CRASH)
#include "Core/CrashCatch_Win32.h"
#endif




int main(int argc, char *argv[])
{
    int result=-1;
    try {
#if defined(WIN32) && defined(ENABLE_CRASH)
        installCashHandler();
#endif
        PH::SharedPtr<kcdz::SystemLogger>     ptrLogger=new kcdz::SystemLogger();
        ptrLogger->setLoggerName(GlobalSetting::getInstance()->getLoggerName());
        ptrLogger->setLoggerPath(GlobalSetting::getInstance()->getLoggerPath());
        ptrLogger->setEnableConsle(GlobalSetting::getInstance()->getEnableConsle());
        ptrLogger->setEnableFile(GlobalSetting::getInstance()->getEnableFile());
        ptrLogger->init();

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
        QCoreApplication::setOrganizationName("kcdzApp");
        kcdz::Application app(argc, argv, kcdz::Application::WidgetApplication);
        kcdz::Application::setApplicationId("kcdzApp");
        if(app.sendMessage("kcdzAppStart"))  return 0;

        QDir::setCurrent(kcdz::Application::appPath());
        qDebug()<<"*********************************App Start**************************";
        if(!DBManager::getInstance()->init()){
            QMessageBox::warning(nullptr,QStringLiteral("警告"),QStringLiteral("数据库初始化失败!"));
            return -1;
        }
        MainWindow wnd;
        wnd.showNormal();

        result=app.exec();
    } catch (std::exception& exc) {
        qCritical()<<exc.what();
    }
    return result;
}
