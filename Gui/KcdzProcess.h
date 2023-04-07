#ifndef KCDZPROCESS_H
#define KCDZPROCESS_H

#include <QObject>
#include <QStringList>

namespace kcdz{
class  KcdzProcess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments NOTIFY argumentsChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

    class Private;

public:
    KcdzProcess(QObject *parent = Q_NULLPTR);
    virtual ~KcdzProcess();

    void setCommand(const QString &command);
    QString command() const;

    void setArguments(const QStringList &arguments);
    QStringList arguments() const;

    bool running() const;

Q_SIGNALS:
    void commandChanged();
    void argumentsChanged();
    void runningChanged();
    void standardOutput(const QString &output);
    void errorOutput(const QString &output);

public Q_SLOTS:
    void start();
    void terminate();

private:
    Private *p;
};
}

#endif // KCDZPROCESS_H
