#ifndef SQLOBJECT_H
#define SQLOBJECT_H

#include <QObject>
#include <QVariantMap>
#include <QThread>

///stl
#include <functional>
class QSqlQuery;
namespace kcdz {
class SqlObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString table READ table WRITE setTable NOTIFY tableChanged)
    Q_PROPERTY(QVariant driver READ driver WRITE setDriver NOTIFY driverChanged)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setDatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(qint32 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString createQuery READ createQuery WRITE setCreateQuery NOTIFY createQueryChanged)
    Q_PROPERTY(QStringList primaryKeys READ primaryKeys WRITE setPrimaryKeys NOTIFY primaryKeysChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_ENUMS(DriverTypes)

    class Private;
    friend class AsemanSqlObjectAsync;
public:
    enum DriverTypes {
        NoneDriver,
        SQLite,
        MySQL,
        PostgreSQL,
        ODBC,
        MSSQL
    };

    class Core;

    explicit SqlObject(QObject *parent = nullptr);
    virtual ~SqlObject();

    void setTable(const QString &table);
    QString table() const;

    void setPrimaryKeys(const QStringList &primaryKeys);
    QStringList primaryKeys() const;

    void setDriver(const QVariant &driver);
    QVariant driver() const;

    QString databaseName() const;
    void setDatabaseName(const QString &databaseName);

    QString host() const;
    void setHost(const QString &host);

    qint32 port() const;
    void setPort(qint32 port);

    QString userName() const;
    void setUserName(const QString &userName);

    QString password() const;
    void setPassword(const QString &password);

    QString createQuery() const;
    void setCreateQuery(const QString &createQuery);

    QString lastError() const;

    void queryAsync(const QString &query, const QVariantMap &binds, std::function<void (QVariantList result, const QString &error)> callback);


    qint32 insert(const QString &extra = QString());
    void update(const QString &where = QString(), const QVariantMap &binds = QVariantMap());

    QVariantList select();
    QVariantList select(const QString &where, const QVariantMap &binds) { return select("", where, "", binds); }
    QVariantList select(const QString join, const QString &where, const QVariantMap &binds) { return select(join, where, "", binds); }
    QVariantList select(const QString join, const QString &where, const QString &extra, const QVariantMap &binds);

    void push() { insert(); }
    void fetch();

    void deleteQuery(const QString &where, const QVariantMap &binds = QVariantMap());

    QVariantList query(const QString &query, const QVariantMap &binds = QVariantMap());

    void create();
    void drop();

    QVariantMap getData();

    static QVariantList generateResult(class QSqlQuery &res);

Q_SIGNALS:
    void tableChanged();
    void databaseNameChanged();
    void hostChanged();
    void portChanged();
    void driverChanged();
    void userNameChanged();
    void passwordChanged();
    void primaryKeysChanged();
    void createQueryChanged();
    void lastErrorChanged();

protected:
    QVariantMap prepareBinds(const QVariantMap &binds) const;

private:
    void initialize();
    Core *init(const QString &forceConnectionName = QString());
    void refreshKey();

    void setLastError(const QString &lastError);
#ifdef QT_SQL_LIB
    int queryExec(class QSqlQuery &q);
#endif

    QStringList properties() const;

private:
    Private *p;
};
class SqlObjectAsync: public QThread
{
    Q_OBJECT
public:
    SqlObjectAsync(SqlObject::Core *core, const QString &query, const QVariantMap &binds, QObject *parent = Q_NULLPTR);
    virtual ~SqlObjectAsync();

Q_SIGNALS:
    void error(const QString &error);
    void result(const QVariantList &result);

protected:
    void run() override;

private:
    SqlObject::Core *mCore;
    QString mQuery;
    QVariantMap mBinds;
};
}


#endif // SQLOBJECT_H
