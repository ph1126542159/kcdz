#include "KcdzProcess.h"
#include <QPointer>
#include <QProcess>
namespace kcdz {
class KcdzProcess::Private
{
public:
    QString command;
    QStringList arguments;
    QPointer<QProcess> process;
};

KcdzProcess::KcdzProcess(QObject *parent) :
    QObject(parent)
{
    p = new Private;
}

void KcdzProcess::setCommand(const QString &command)
{
    if(p->command == command)
        return;

    p->command = command;
    Q_EMIT commandChanged();
}

QString KcdzProcess::command() const
{
    return p->command;
}

void KcdzProcess::setArguments(const QStringList &arguments)
{
    if(p->arguments == arguments)
        return;

    p->arguments = arguments;
    Q_EMIT argumentsChanged();
}

void KcdzProcess::start()
{
    if(p->process)
        p->process->terminate();

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::ForwardedChannels);

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    connect(process, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [process](int){
#else
    connect(process, &QProcess::finished, this, [process](int){
#endif
        process->deleteLater();
    });
    connect(process, &QProcess::destroyed, this, [this, process](){
        if(!p->process || p->process == process)
            Q_EMIT runningChanged();
    });
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process](){
        Q_EMIT standardOutput(QString::fromUtf8(process->readAllStandardOutput()));
    });
    connect(process, &QProcess::readAllStandardError, this, [this, process](){
        Q_EMIT standardOutput(QString::fromUtf8(process->readAllStandardError()));
    });

    p->process = process;
    p->process->start(p->command, p->arguments);

    Q_EMIT runningChanged();
}

void KcdzProcess::terminate()
{
    if(!p->process)
        return;

    p->process->terminate();
    Q_EMIT runningChanged();
}

QStringList KcdzProcess::arguments() const
{
    return p->arguments;
}

bool KcdzProcess::running() const
{
    return p->process;
}

KcdzProcess::~KcdzProcess()
{
    delete p;
}

}
