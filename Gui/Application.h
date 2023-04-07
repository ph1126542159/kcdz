#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QFont>
#include <QVariant>
#include <QCoreApplication>
#include <QClipboard>
#include <QUrl>

#ifdef QT_GUI_LIB
#include <QIcon>
#endif
namespace kcdz {

class NetworkProxy;
class QSettings;
class ApplicationPrivate;

class Application : public QObject
{
    Q_OBJECT
    Q_ENUMS(ApplicationType)
    Q_ENUMS(ApplicationState)

    Q_PROPERTY(QString homePath     READ homePath     NOTIFY homePathChanged)
    Q_PROPERTY(QString startPath    READ startPath    NOTIFY fakeSignal)
    Q_PROPERTY(QString appPath      READ appPath      NOTIFY fakeSignal)
    Q_PROPERTY(QString appFilePath  READ appFilePath  NOTIFY fakeSignal)
    Q_PROPERTY(QString confsPath    READ confsPath    NOTIFY confsPathChanged)
    Q_PROPERTY(QString tempPath     READ tempPath     NOTIFY fakeSignal)
    Q_PROPERTY(QString backupsPath  READ backupsPath  NOTIFY backupsPathChanged)
    Q_PROPERTY(QString cameraPath   READ cameraPath   NOTIFY fakeSignal)

    Q_PROPERTY(int appType READ appType NOTIFY fakeSignal)
    Q_PROPERTY(bool isDebug READ isDebug NOTIFY fakeSignal)

    Q_PROPERTY(QFont globalFont READ globalFont WRITE setGlobalFont NOTIFY globalFontChanged)

    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName NOTIFY applicationNameChanged)
    Q_PROPERTY(QString applicationVersion READ applicationVersion WRITE setApplicationVersion NOTIFY applicationVersionChanged)
    Q_PROPERTY(QString organizationName READ organizationName WRITE setOrganizationName NOTIFY organizationNameChanged)
    Q_PROPERTY(QString organizationDomain READ organizationDomain WRITE setOrganizationDomain NOTIFY organizationDomainChanged)
    Q_PROPERTY(QString applicationAbout READ applicationAbout WRITE setApplicationAbout NOTIFY applicationAboutChanged)
    Q_PROPERTY(bool qpaNoTextHandles READ qpaNoTextHandles WRITE setQpaNoTextHandles NOTIFY qpaNoTextHandlesChanged)
    Q_PROPERTY(QUrl windowIcon READ windowIcon WRITE setWindowIcon NOTIFY windowIconChanged)

    Q_PROPERTY(int applicationState READ applicationState NOTIFY applicationStateChanged)

    Q_PROPERTY(QString applicationDisplayName READ applicationDisplayName WRITE setApplicationDisplayName NOTIFY fakeSignal)
    Q_PROPERTY(QString platformName READ platformName STORED false)
    Q_PROPERTY(bool quitOnLastWindowClosed  READ quitOnLastWindowClosed WRITE setQuitOnLastWindowClosed NOTIFY fakeSignal)

public:
    enum ApplicationType {
        NoneApplication,
#ifdef QT_GUI_LIB
        GuiApplication,
#endif
#ifdef QT_CORE_LIB
        CoreApplication,
#endif
#ifdef QT_WIDGETS_LIB
        WidgetApplication
#endif
    };

    enum ApplicationState {
        ApplicationSuspended    = Qt::ApplicationSuspended,
        ApplicationHidden       = Qt::ApplicationHidden,
        ApplicationInactive     = Qt::ApplicationInactive,
        ApplicationActive       = Qt::ApplicationActive
    };

    Application(int &argc, char **argv, ApplicationType appType);
    virtual ~Application();

    static QString homePath();
    static void setHomePath(const QString &path);

    static QString startPath();
    static QString appPath();
    static QString appFilePath();
    static QString confsPath();
    static QString tempPath();
    static QString backupsPath();
    static QString cameraPath();

    static QString applicationDirPath();
    static QString applicationFilePath();
    static qint64 applicationPid();

    static void setOrganizationDomain(const QString &orgDomain);
    static QString organizationDomain();
    static void setOrganizationName(const QString &orgName);
    static QString organizationName();
    static void setApplicationName(const QString &application);
    static QString applicationName();
    static void setApplicationVersion(const QString &version);
    static QString applicationVersion();
    static void setApplicationDisplayName(const QString &name);
    static QString applicationDisplayName();
    static void setApplicationAbout(const QString &desc);
    static QString applicationAbout();

    static void setQpaNoTextHandles(bool stt);
    static bool qpaNoTextHandles();

    static QString platformName();

    static QStringList arguments();

    static void setQuitOnLastWindowClosed(bool quit);
    static bool quitOnLastWindowClosed();

    static QClipboard *clipboard();

    void setWindowIcon(const QUrl &icon);
    QUrl windowIcon() const;

    static int appType();
    static bool isDebug();

    static Application *instance();
    static QCoreApplication *qapp();

    void setGlobalFont(const QFont &font);
    QFont globalFont() const;

    static QFont font();
    static void setFont(const QFont &f);

#ifdef QT_GUI_LIB
    static QPalette palette();
    static void setPalette(const QPalette &pal);

    static int applicationState();
#endif

    static void setApplicationId(const QString &applicationId);
    static QString applicationId();

    static bool isRunning();

    inline operator QCoreApplication*() const { return qapp(); }

    bool checkPermissions(const QString &persmission);
public Q_SLOTS:
    bool sendMessage(const QString &message, int timeout = 5000);
    void refreshTranslations();
    void back();
    int exec();
    static void exit(int retcode = 0);
    void sleep(quint64 ms);
Q_SIGNALS:
    void messageReceived(const QString &msg);
    void proxyChanged();
    void fakeSignal();
    void globalFontFamilyChanged();
    void globalMonoFontFamilyChanged();
    void globalFontChanged();
    void languageUpdated();
    void backRequest();
    void clickedOnDock();

    void homePathChanged();
    void confsPathChanged();
    void backupsPathChanged();

    void organizationNameChanged();
    void organizationDomainChanged();
    void applicationNameChanged();
    void applicationVersionChanged();
    void applicationStateChanged();
    void applicationAboutChanged();
    void windowIconChanged();
    void qpaNoTextHandlesChanged();

    void lastWindowClosed();

protected:

private:
    void init();

private:
    ApplicationPrivate *p;

};
}


#endif // APPLICATION_H
