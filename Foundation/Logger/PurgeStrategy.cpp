#include "PurgeStrategy.h"
#include <QDir>
#include <assert.h>
#include <QDebug>
namespace PH {

PurgeStrategy::PurgeStrategy()
{
}


PurgeStrategy::~PurgeStrategy()
{
}

void PurgeStrategy::list(const QString &path, std::vector<QFileInfo> &files)
{
    QDir p(path);
    QString baseName=QFileInfo(path).fileName();
    p.cdUp();

    const QFileInfoList list = p.entryInfoList();
    foreach (auto it, list) {
        if(it.fileName().contains(baseName))
            files.push_back(it);
    }
}

PurgeByAgeStrategy::PurgeByAgeStrategy(const qint64 &age):
    _age(age)
{

}

PurgeByAgeStrategy::~PurgeByAgeStrategy()
{

}

void PurgeByAgeStrategy::purge(const QString &path)
{
    std::vector<QFileInfo> files;
    list(path,files);
    for (auto& f: files)
    {
        qint64 now=QDateTime::currentDateTime().toMSecsSinceEpoch();
        qint64 t=now-f.lastModified().toMSecsSinceEpoch();
        if (t>=_age)
        {
            QFile(f.absoluteFilePath()).remove();
        }
    }
}

PurgeByCountStrategy::PurgeByCountStrategy(std::size_t count): _count(count)
{
    assert(count > 0);
}

PurgeByCountStrategy::~PurgeByCountStrategy()
{

}

void PurgeByCountStrategy::purge(const QString& path)
{
    std::vector<QFileInfo> files;
    list(path, files);
    while (files.size() > _count)
    {
        std::vector<QFileInfo>::iterator it = files.begin();
        std::vector<QFileInfo>::iterator purgeIt = it;
        QDateTime purgeTS = purgeIt->lastModified();
        ++it;
        while (it != files.end())
        {
            QDateTime md(it->lastModified());
            if (md <= purgeTS)
            {
                purgeTS = md;
                purgeIt = it;
            }
            ++it;
        }
        QFile((*purgeIt).absolutePath()).remove();
        files.erase(purgeIt);
    }
}
}
