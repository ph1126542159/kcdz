#include "Application.h"
#include <QDir>
#include <QFont>
#include <QPalette>
#include <QSettings>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include <QPointer>
#include "QtSingleApplication/qtlocalpeer.h"

#ifdef QT_GUI_LIB
#define READ_DEFINITION(FUNCTION, DEFAULT_VALUE) \
    switch(kcdz_app_singleton->p->appType) { \
    case GuiApplication: \
    return static_cast<QGuiApplication*>(QCoreApplication::instance())->FUNCTION(); \
    break; \
    default: \
    return DEFAULT_VALUE; \
    break; \
    }
#else
#define READ_DEFINITION(FUNCTION, DEFAULT_VALUE) \
    return DEFAULT_VALUE;
#endif

#ifdef QT_GUI_LIB
#define SET_DIFINITION(FUNCTION, VALUE) \
    switch(kcdz_app_singleton->p->appType) { \
    case GuiApplication: \
    static_cast<QGuiApplication*>(QCoreApplication::instance())->FUNCTION(VALUE); \
    break; \
    }
#else
#define SET_DIFINITION(FUNCTION, VALUE)
Q_UNUSED(VALUE)
#endif

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

#ifdef QT_GUI_LIB
#include <QGuiApplication>
#endif
#ifdef QT_CORE_LIB
#include <QCoreApplication>
#endif
#ifdef QT_WIDGETS_LIB
#include <QApplication>
#endif

static kcdz::Application *kcdz_app_singleton = 0;
static QSet<kcdz::Application*> kcdz_app_objects;
static QString *kcdz_app_home_path = 0;
static QString *kcdz_app_tmp_path = 0;

#if defined(Q_OS_MAC) && defined(Q_PROCESSOR_X86_32)
#include <objc/objc.h>
#include <objc/message.h>

bool dockClickHandler(id self,SEL _cmd,...)
{
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    if(kcdz_app_singleton) kcdz_app_singleton->clickedOnDock();
    return true;
}
#endif
namespace kcdz {
class ApplicationPrivate
{
public:
    QUrl windowIcon;
    QTimer *clickOnDock_timer;
    QFont globalFont;
    int appType;
    QCoreApplication *app;
    bool app_owner;
    QString appAbout;
    static QtLocalPeer *peer;
    kcdz::NetworkProxy* proxy=nullptr;
};

QtLocalPeer* ApplicationPrivate::peer=nullptr;

Application::Application(int &argc, char **argv, ApplicationType appType) :
    QObject()
{
    if(!kcdz_app_singleton)
        kcdz_app_singleton = this;

    p = new ApplicationPrivate;
    p->appType = appType;
    p->app_owner = true;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    switch(p->appType)
    {
    case CoreApplication:
#ifdef QT_CORE_LIB
        p->app = new QCoreApplication(argc, argv);
#endif
        break;
    case GuiApplication:
#ifdef QT_GUI_LIB
        p->app = new QGuiApplication(argc, argv);
#endif
        break;
    case WidgetApplication:
#ifdef QT_WIDGETS_LIB
        p->app=new QApplication(argc, argv);
#endif
        break;

    default:
        p->app = 0;
        break;
    }

    kcdz_app_objects.insert(this);

    init();
}

void Application::init()
{
    switch(p->appType)
    {
#ifdef QT_GUI_LIB
    case GuiApplication:
        connect(static_cast<QGuiApplication*>(p->app), &QGuiApplication::lastWindowClosed, this, &Application::lastWindowClosed);
        connect(static_cast<QGuiApplication*>(p->app), &QGuiApplication::applicationStateChanged, this, &Application::applicationStateChanged);
        p->globalFont = static_cast<QGuiApplication*>(p->app)->font();
#endif
#ifdef QT_CORE_LIB
    case CoreApplication:
        connect(p->app, &QCoreApplication::organizationNameChanged, this, &Application::organizationNameChanged);
        connect(p->app, &QCoreApplication::organizationDomainChanged, this, &Application::organizationDomainChanged);
        connect(p->app, &QCoreApplication::applicationNameChanged, this, &Application::applicationNameChanged);
        connect(p->app, &QCoreApplication::applicationVersionChanged, this, &Application::applicationVersionChanged);
        break;
#endif

    default:
        p->app = 0;
        break;
    }

    p->clickOnDock_timer = new QTimer(this);
    p->clickOnDock_timer->setSingleShot(true);
    p->clickOnDock_timer->setInterval(500);

#if defined(Q_OS_MAC) && defined(Q_PROCESSOR_X86_32)
#ifndef Q_OS_IOS
    objc_object* cls = objc_getClass("NSApplication");
    SEL sharedApplication = sel_registerName("sharedApplication");
    objc_object* appInst = objc_msgSend(cls,sharedApplication);

    if(appInst != NULL)
    {
        objc_object* delegate = objc_msgSend(appInst, sel_registerName("delegate"));
        objc_object* delClass = objc_msgSend(delegate,  sel_registerName("class"));
        const char* tst = class_getName(delClass->isa);
        bool test = class_addMethod((objc_class*)delClass, sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:"), (IMP)dockClickHandler,"B@:");

        Q_UNUSED(tst)
        if (!test)
        {
            // failed to register handler...
        }
    }
#endif
#endif
}

QString Application::homePath()
{
    if(kcdz_app_home_path)
        return *kcdz_app_home_path;

    kcdz_app_home_path = new QString();

    QString oldPath;
#ifdef Q_OS_WIN
    oldPath = QDir::homePath() + "/AppData/Local/" + QCoreApplication::applicationName();
#else
    oldPath = QDir::homePath() + "/.config/" + QCoreApplication::applicationName();
#endif

    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::QStandardPaths::AppDataLocation);
    if(paths.isEmpty())
        paths << oldPath;

    if( oldPath.count() && QFileInfo::exists(oldPath) )
        *kcdz_app_home_path = oldPath;
    else
        *kcdz_app_home_path = paths.first();

    QDir().mkpath(*kcdz_app_home_path);
    return *kcdz_app_home_path;
}
void Application::setApplicationId(const QString &applicationId){
    if(ApplicationPrivate::peer) return;
    ApplicationPrivate::peer=new QtLocalPeer(instance(), applicationId);
    connect(ApplicationPrivate::peer, SIGNAL(messageReceived(const QString&)), instance(),SIGNAL(messageReceived(const QString&)));
}
QString Application::applicationId(){
    if(ApplicationPrivate::peer)  ApplicationPrivate::peer->applicationId();
    return "";
}
bool Application::sendMessage(const QString &message, int timeout)
{
    if(ApplicationPrivate::peer)
        return ApplicationPrivate::peer->sendMessage(message, timeout);
    return false;
}
bool Application::isRunning(){
    if(ApplicationPrivate::peer) return ApplicationPrivate::peer->isClient();
    return false;
}
void Application::setHomePath(const QString &path)
{
    homePath();
    *kcdz_app_home_path = path;

    if(kcdz_app_singleton)
    {
        Q_EMIT kcdz_app_singleton->homePathChanged();
        Q_EMIT kcdz_app_singleton->confsPathChanged();
        Q_EMIT kcdz_app_singleton->backupsPathChanged();
    }
}

QString Application::startPath()
{
#ifdef Q_OS_ANDROID
    return QStringLiteral("/sdcard/");
#else
#ifdef Q_OS_IOS
    return homePath();
#else
    return QDir::homePath();
#endif
#endif
}

QString Application::appPath()
{
    return QCoreApplication::applicationDirPath();
}

QString Application::appFilePath()
{
    return QCoreApplication::applicationFilePath();
}

QString Application::confsPath()
{
    return homePath() + "/config.ini";
}

QString Application::tempPath()
{
    if(!kcdz_app_tmp_path)
    {
        kcdz_app_tmp_path = new QString();
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
        *kcdz_app_tmp_path = homePath() + "/tmp/";
#else
        *kcdz_app_tmp_path = QDir::tempPath();
#endif
        QDir().mkpath(*kcdz_app_tmp_path);
    }

    return *kcdz_app_tmp_path;
}

QString Application::backupsPath()
{
#ifdef Q_OS_ANDROID
    return "/sdcard/" + QCoreApplication::organizationDomain() + "/" + QCoreApplication::applicationName() + "/backups";
#else
#ifdef Q_OS_IOS
    return homePath() + "/backups/";
#else
    return homePath() + "/backups";
#endif
#endif
}

QString Application::cameraPath()
{
#ifdef Q_OS_ANDROID
    return "/sdcard/DCIM";
#else
#ifdef Q_OS_IOS
    return QStandardPaths::standardLocations(QStandardPaths::QStandardPaths::AppDataLocation).first() + "/camera/";
#else
    return QDir::homePath() + "/Pictures/Camera";
#endif
#endif
}

QString Application::applicationDirPath()
{
    return QCoreApplication::applicationDirPath();
}

QString Application::applicationFilePath()
{
    return QCoreApplication::applicationFilePath();
}

qint64 Application::applicationPid()
{
    return QCoreApplication::applicationPid();
}

void Application::setOrganizationDomain(const QString &orgDomain)
{
    QCoreApplication::setOrganizationDomain(orgDomain);
}

QString Application::organizationDomain()
{
    return QCoreApplication::organizationDomain();
}

void Application::setOrganizationName(const QString &orgName)
{
    QCoreApplication::setOrganizationName(orgName);
}

QString Application::organizationName()
{
    return QCoreApplication::organizationName();
}

void Application::setApplicationName(const QString &application)
{
    QCoreApplication::setApplicationName(application);
}

QString Application::applicationName()
{
    return QCoreApplication::applicationName();
}

void Application::setApplicationVersion(const QString &version)
{
    QCoreApplication::setApplicationVersion(version);
}

QString Application::applicationVersion()
{
    return QCoreApplication::applicationVersion();
}

void Application::setApplicationDisplayName(const QString &name)
{
    SET_DIFINITION(setApplicationDisplayName, name)
}

QString Application::applicationDisplayName()
{
    READ_DEFINITION(applicationDisplayName, QString())
}

void Application::setApplicationAbout(const QString &desc)
{
    if(!kcdz_app_singleton)
        return;
    if( kcdz_app_singleton->p->appAbout == desc )
        return;

    kcdz_app_singleton->p->appAbout = desc;
    Q_EMIT kcdz_app_singleton->applicationAboutChanged();
}

QString Application::applicationAbout()
{
    if(kcdz_app_singleton)
        return tr(kcdz_app_singleton->p->appAbout.toUtf8());
    else
        return QString();
}

void Application::setQpaNoTextHandles(bool stt)
{
    bool currentResult = qpaNoTextHandles();
    if(currentResult == stt)
        return;

    if(stt)
    {
        qputenv("QT_ANDROID_ENABLE_WORKAROUND_TO_DISABLE_PREDICTIVE_TEXT", "1");
        qputenv("QT_QPA_NO_TEXT_HANDLES", "1");
    }
    else
    {
        qunsetenv("QT_QPA_NO_TEXT_HANDLES");
        qunsetenv("QT_ANDROID_ENABLE_WORKAROUND_TO_DISABLE_PREDICTIVE_TEXT");
    }

    Q_EMIT kcdz_app_singleton->qpaNoTextHandlesChanged();
}

bool Application::qpaNoTextHandles()
{
    return qgetenv("QT_QPA_NO_TEXT_HANDLES") == "1";
}

QString Application::platformName()
{
    READ_DEFINITION(platformName, QString())
}

QStringList Application::arguments()
{
    return QCoreApplication::arguments();
}

void Application::setQuitOnLastWindowClosed(bool quit)
{
    SET_DIFINITION(setQuitOnLastWindowClosed, quit)
}

bool Application::quitOnLastWindowClosed()
{
    READ_DEFINITION(quitOnLastWindowClosed, false)
}

QClipboard *Application::clipboard()
{
    READ_DEFINITION(clipboard, 0)
}

void Application::setWindowIcon(const QUrl &icon)
{
    if(p->windowIcon == icon)
        return;

    p->windowIcon = icon;
    QString file = icon.toString();
    if(file.left(6) == QStringLiteral("qrc://"))
        file = file.replace(0, 6, QStringLiteral(":/"));

#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
    SET_DIFINITION(setWindowIcon, QIcon(file))
        #else
    Q_UNUSED(icon)
#endif
    Q_EMIT windowIconChanged();
}

QUrl Application::windowIcon() const
{
    return p->windowIcon;
}

int Application::appType()
{
    return kcdz_app_singleton->p->appType;
}

bool Application::isDebug()
{
#ifdef QT_DEBUG
    return true;
#else
    return false;
#endif
}

Application *Application::instance()
{
    return kcdz_app_singleton;
}

QCoreApplication *Application::qapp()
{
    return QCoreApplication::instance();
}

void Application::setGlobalFont(const QFont &font)
{
    if(p->globalFont == font)
        return;

    p->globalFont = font;
    Q_EMIT globalFontChanged();
}

QFont Application::globalFont() const
{
    return p->globalFont;
}

QFont Application::font()
{
    READ_DEFINITION(font, QFont())
}

void Application::setFont(const QFont &f)
{
    SET_DIFINITION(setFont, f);
}

#ifdef QT_GUI_LIB
QPalette Application::palette()
{
    READ_DEFINITION(palette, QPalette())
}

void Application::setPalette(const QPalette &pal)
{
    SET_DIFINITION(setPalette, pal);
}

int Application::applicationState()
{
    READ_DEFINITION(applicationState, Qt::ApplicationActive)
}
#endif

void Application::refreshTranslations()
{
    Q_EMIT languageUpdated();
}

void Application::back()
{
    Q_EMIT backRequest();
}

int Application::exec()
{
    p->clickOnDock_timer->stop();
    p->clickOnDock_timer->start();

    return p->app->exec();
}

void Application::exit(int retcode)
{
    kcdz_app_singleton->p->app->exit(retcode);
}

void Application::sleep(quint64 ms)
{
    QThread::msleep(ms);
}
#ifdef QT_QML_LIB
bool Application::checkPermissions(const QString &persmission)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)) && defined (Q_OS_ANDROID)
    if (QtAndroid::checkPermission(persmission) == QtAndroid::PermissionResult::Granted)
        return true;
    else
        return false;
#else
    Q_UNUSED(persmission)
    return true;
#endif
}

bool Application::sendMessage(const QString &message, int timeout)
{
    if(ApplicationPrivate::peer)
        return ApplicationPrivate::peer->sendMessage(message, timeout);
    return false;
}
#endif

Application::~Application()
{
    kcdz_app_objects.remove(this);
    if(kcdz_app_singleton == this)
        kcdz_app_singleton = 0;

    if(p->app && p->app_owner)
        delete p->app;

    if(ApplicationPrivate::peer)
    {
        delete ApplicationPrivate::peer;
        ApplicationPrivate::peer = nullptr;
    }

    delete p;
}
}

