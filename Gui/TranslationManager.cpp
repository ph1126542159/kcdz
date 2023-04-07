#include "TranslationManager.h"
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>

#include "Tools.h"
#include "GlobalTranslations.h"
namespace kcdz {
class TranslationManager::Private
{
public:
    QTranslator *translator;
    QUrl sourceDirectory;
    QString localeName;
    QString fileName;
    QString delimiters;
};

TranslationManager::TranslationManager(QObject *parent) :
    QObject(parent)
{
    p = new Private;
    p->translator = 0;
}

void TranslationManager::setSourceDirectory(const QUrl &sourceDirectory)
{
    if(p->sourceDirectory == sourceDirectory)
        return;

    p->sourceDirectory = sourceDirectory;
    refresh();
    Q_EMIT sourceDirectoryChanged();
}

QUrl TranslationManager::sourceDirectory() const
{
    return p->sourceDirectory;
}

void TranslationManager::setLocaleName(const QString &localeName)
{
    if(p->localeName == localeName)
        return;

    p->localeName = localeName;
    refresh();
    Q_EMIT localeNameChanged();
}

QString TranslationManager::localeName() const
{
    return p->localeName;
}

void TranslationManager::setFileName(const QString &fileName)
{
    if(p->fileName == fileName)
        return;

    p->fileName = fileName;
    refresh();
    Q_EMIT fileNameChanged();
}

QString TranslationManager::fileName() const
{
    return p->fileName;
}

void TranslationManager::setDelimiters(const QString &delimiters)
{
    if(p->delimiters == delimiters)
        return;

    p->delimiters = delimiters;
    refresh();
    Q_EMIT delimitersChanged();
}

QString TranslationManager::delimiters() const
{
    return p->delimiters;
}

int TranslationManager::textDirection() const
{
    return QLocale(p->localeName).textDirection();
}

QMap<QString, QVariant> TranslationManager::translations() const
{
    QMap<QString, QVariant> res;
    QString path = Tools::urlToLocalPath(p->sourceDirectory);

    QRegularExpression rx(p->fileName + p->delimiters + QStringLiteral("(\\w+)\\.qm"));
    QStringList files = QDir(path).entryList(QStringList()<<QStringLiteral("*.qm"), QDir::Files);
    for(const QString &file: files)
    {
        auto i = rx.match(file);
        if (!i.hasMatch())
            continue;

        QString cap = i.captured(1);
        res[cap] = QLocale(cap).nativeLanguageName();
    }

    return res;
}

void TranslationManager::refresh()
{
    if(p->sourceDirectory.isEmpty() || p->fileName.isEmpty() || p->delimiters.isEmpty() || p->localeName.isEmpty())
        return;

    QString path = Tools::urlToLocalPath(p->sourceDirectory);
    if(p->translator)
    {
        QCoreApplication::removeTranslator(p->translator);
        delete p->translator;
    }

    p->translator = new QTranslator(this);
    p->translator->load( QLocale(p->localeName), p->fileName, p->delimiters, path);

    QCoreApplication::installTranslator(p->translator);

    Q_EMIT refreshed();
    Q_EMIT textDirectionChanged();
    Q_EMIT translationsChanged();
    GlobalTranslations::refresh();
}

TranslationManager::~TranslationManager()
{
    if(p->translator)
        QCoreApplication::removeTranslator(p->translator);

    delete p;
}
}
