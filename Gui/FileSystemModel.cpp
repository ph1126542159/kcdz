#include "FileSystemModel.h"
#include <QFileSystemWatcher>
#include <QDir>
#include <QMimeData>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDateTime>
#include <QFileInfo>
#include <QTimerEvent>
#include <QTimer>
#include <QDebug>
#include <QUrl>
namespace kcdz {
class FileSystemModelPrivate
{
public:
    bool showDirs;
    bool showDotAndDotDot;
    bool showDirsFirst;
    bool showFiles;
    bool showHidden;
    bool recursive;
    QStringList nameFilters;
    QString folder;
    int sortField;
    qint32 limit;

    QList<QFileInfo> list;
    QMimeDatabase mdb;

    QFileSystemWatcher *watcher;
    QTimer *refresh_timer;
};

class SortUnitType
{
public:
    SortUnitType(): num(0){}
    QChar ch;
    quint64 num;
};

QList<SortUnitType> aseman_analize_file_name(const QString &fileName)
{
    QList<SortUnitType> res;
    for(int i=0; i<fileName.length(); i++)
    {
        const QChar &ch = fileName[i];
        if(ch.isNumber())
        {
            int num = QString(ch).toInt();
            if(res.isEmpty() || !res.last().ch.isNull() )
                res << SortUnitType();

            SortUnitType & resUnit = res[res.length()-1];
            resUnit.num = resUnit.num*10 + num;
        }
        else
        {
            SortUnitType unit;
            unit.ch = ch;
            res << unit;
        }
    }

    return res;
}

FileSystemModelPrivate *fileListSort_private_data = Q_NULLPTR;
bool aseman_fileListSort(const QFileInfo &f1, const QFileInfo &f2)
{
    if(fileListSort_private_data->sortField == FileSystemModel::Size)
        return f1.size() < f2.size();
    if(fileListSort_private_data->sortField == FileSystemModel::Date)
        return f1.lastModified() > f2.lastModified();

    if(fileListSort_private_data->showDirsFirst)
    {
        if(f1.isDir() && !f2.isDir())
            return true;
        if(!f1.isDir() && f2.isDir())
            return false;
    }

    const QString & s1 = f1.fileName();
    const QString & s2 = f2.fileName();

    const QList<SortUnitType> &ul1 = aseman_analize_file_name(s1);
    const QList<SortUnitType> &ul2 = aseman_analize_file_name(s2);

    for(int i=0; i<ul1.length() && i<ul2.length(); i++)
    {
        const SortUnitType &u1 = ul1.at(i);
        const SortUnitType &u2 = ul2.at(i);

        if(u1.ch.isNull() && !u2.ch.isNull())
            return true;
        if(!u1.ch.isNull() && u2.ch.isNull())
            return false;
        if(!u1.ch.isNull() && !u2.ch.isNull())
        {
            if(u1.ch < u2.ch)
                return true;
            if(u1.ch > u2.ch)
                return false;
        }
        if(u1.ch.isNull() && u2.ch.isNull())
        {
            if(u1.num < u2.num)
                return true;
            if(u1.num > u2.num)
                return false;
        }
    }

    return ul1.length() < ul2.length();
}



FileSystemModel::FileSystemModel(QObject *parent) :
    AbstractListModel(parent)
{
    p = new FileSystemModelPrivate;
    p->showDirs = true;
    p->showDotAndDotDot = false;
    p->showDirsFirst = true;
    p->showFiles = true;
    p->showHidden = false;
    p->sortField = FileSystemModel::Name;
    p->refresh_timer = Q_NULLPTR;
    p->recursive = false;
    p->limit = 0;

    p->watcher = new QFileSystemWatcher(this);

    p->refresh_timer = new QTimer(this);
    p->refresh_timer->setInterval(10);

    connect(p->watcher, &QFileSystemWatcher::directoryChanged, this, &FileSystemModel::refresh);
    connect(p->watcher, &QFileSystemWatcher::fileChanged, this, &FileSystemModel::refresh);

    connect(p->refresh_timer, &QTimer::timeout, this, &FileSystemModel::reinit_buffer);
}



void FileSystemModel::setShowDirs(bool stt)
{
    if(p->showDirs == stt)
        return;

    p->showDirs = stt;
    Q_EMIT showDirsChanged();

    refresh();
}

bool FileSystemModel::showDirs() const
{
    return p->showDirs;
}


void FileSystemModel::setShowDotAndDotDot(bool stt)
{
    if(p->showDotAndDotDot == stt)
        return;

    p->showDotAndDotDot = stt;
    Q_EMIT showDotAndDotDotChanged();

    refresh();
}

bool FileSystemModel::showDotAndDotDot() const
{
    return p->showDotAndDotDot;
}



void FileSystemModel::setShowDirsFirst(bool stt)
{
    if(p->showDirsFirst == stt)
        return;

    p->showDirsFirst = stt;
    Q_EMIT showDirsFirstChanged();

    refresh();
}

bool FileSystemModel::showDirsFirst() const
{
    return p->showDirsFirst;
}


void FileSystemModel::setShowFiles(bool stt)
{
    if(p->showFiles == stt)
        return;

    p->showFiles = stt;
    Q_EMIT showFilesChanged();

    refresh();
}

bool FileSystemModel::showFiles() const
{
    return p->showFiles;
}


void FileSystemModel::setShowHidden(bool stt)
{
    if(p->showHidden == stt)
        return;

    p->showHidden = stt;
    Q_EMIT showHiddenChanged();

    refresh();
}

bool FileSystemModel::showHidden() const
{
    return p->showHidden;
}


void FileSystemModel::setNameFilters(const QStringList &list)
{
    if(p->nameFilters == list)
        return;

    p->nameFilters = list;
    Q_EMIT nameFiltersChanged();

    refresh();
}

QStringList FileSystemModel::nameFilters() const
{
    return p->nameFilters;
}


void FileSystemModel::setFolder(const QString &url)
{
    if(p->folder == url)
        return;

    if(!p->folder.isEmpty())
        p->watcher->removePath(p->folder);

    p->folder = url;
    if(!p->folder.isEmpty())
        p->watcher->addPath(p->folder);

    Q_EMIT folderChanged();

    refresh();
}


QString FileSystemModel::folder() const
{
    return p->folder;
}


void FileSystemModel::setSortField(int field)
{
    if(p->sortField == field)
        return;

    p->sortField = field;
    Q_EMIT sortFieldChanged();

    refresh();
}

int FileSystemModel::sortField() const
{
    return p->sortField;
}

void FileSystemModel::setRecursive(bool recursive)
{
    if(p->recursive == recursive)
        return;

    p->recursive = recursive;
    Q_EMIT recursiveChanged();

    refresh();
}

bool FileSystemModel::recursive() const
{
    return p->recursive;
}

void FileSystemModel::setLimit(qint32 limit)
{
    if(p->limit == limit)
        return;

    p->limit = limit;
    Q_EMIT limitChanged();
}

qint32 FileSystemModel::limit() const
{
    return p->limit;
}

QString FileSystemModel::parentFolder() const
{
    return QFileInfo(p->folder).dir().absolutePath();
}

const QFileInfo &FileSystemModel::id(const QModelIndex &index) const
{
    return p->list.at( index.row() );
}

int FileSystemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return count();
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    const QFileInfo &info = id(index);
    switch(role)
    {
    case FilePath:
        result = info.filePath();
        break;

    case FileUrl:
        result = QUrl::fromLocalFile(info.filePath());
        break;

    case FileName:
        result = info.fileName();
        break;

    case FileMime:
        result = p->mdb.mimeTypeForFile(info.filePath()).name();
        break;

    case FileSize:
        result = info.size();
        break;

    case FileSuffix:
        result = info.suffix();
        break;

    case FileBaseName:
        result = info.completeBaseName();
        break;

    case FileIsDir:
        result = info.isDir();
        break;

    case FileModifiedDate:
        result = info.lastModified();
        break;

    case FileCreatedDate:
        result = info.birthTime();
        break;
    }

    return result;
}

QHash<qint32, QByteArray> FileSystemModel::roleNames() const
{
    static QHash<qint32, QByteArray> *res = 0;
    if( res )
        return *res;

    res = new QHash<qint32, QByteArray>();
    res->insert( FilePath, "filePath");
    res->insert( FileUrl , "fileUrl");
    res->insert( FileName, "fileName");
    res->insert( FileMime, "fileMime");
    res->insert( FileSize, "fileSize");
    res->insert( FileSuffix, "fileSuffix");
    res->insert( FileBaseName, "fileBaseName");
    res->insert( FileIsDir, "fileIsDir");
    res->insert( FileModifiedDate, "fileModifiedDate");
    res->insert( FileCreatedDate, "fileCreatedDate");

    return *res;
}

int FileSystemModel::count() const
{
    return p->list.count();
}

void FileSystemModel::refresh()
{
    p->refresh_timer->stop();
    p->refresh_timer->start();
}

void FileSystemModel::reinit_buffer()
{
    p->refresh_timer->stop();

    int filter = 0;
    if(p->showDirs)
        filter = filter | QDir::Dirs;
    if(!p->showDotAndDotDot)
        filter = filter | QDir::NoDotAndDotDot;
    if(p->showFiles)
        filter = filter | QDir::Files;
    if(p->showHidden)
        filter = filter | QDir::Hidden;

    qint32 limit = p->limit? p->limit : INT_MAX;
    QList<QFileInfo> res = ls(p->folder, filter, p->recursive, limit);

    if(!p->nameFilters.isEmpty())
        for(int i=0; i<res.count(); i++)
        {
            const QFileInfo &inf = res.at(i);

            QStringList suffixes;
            if(!inf.suffix().isEmpty())
                suffixes << inf.suffix();
            else
                suffixes = p->mdb.mimeTypeForFile(inf.filePath()).suffixes();

            bool founded = inf.isDir();
            for(const QString &sfx: suffixes)
                if(p->nameFilters.contains(QStringLiteral("*.")+sfx, Qt::CaseInsensitive))
                {
                    founded = true;
                    break;
                }

            if(!founded)
            {
                res.removeAt(i);
                i--;
            }
        }

    fileListSort_private_data = p;
    std::stable_sort(res.begin(), res.end(), aseman_fileListSort);

    changed(res);
}

QList<QFileInfo> FileSystemModel::ls(const QString &path, qint32 filter, bool recursive, qint32 &limit)
{
    QList<QFileInfo> res;

    QDir::SortFlags sort = QDir::Name;
    switch(p->sortField)
    {
    case Size:
        sort = QDir::Size;
        break;
    case Date:
        sort = (QDir::Time | QDir::Reversed);
        break;
    }

    QStringList list;
    if(filter && !path.isEmpty())
        list = QDir(path).entryList(static_cast<QDir::Filter>(filter) | QDir::Dirs, sort);

    for(const QString &fileName: list)
    {
        QFileInfo info(path + "/" + fileName);
        if(!info.isDir() || (static_cast<QDir::Filter>(filter) & QDir::Dirs))
        {
            res << info;
            limit--;
        }
        if(limit < 0)
            return res;

        if(info.isDir() && recursive)
            res << ls(path + "/" + fileName, filter, recursive, limit);
    }

    return res;
}

void FileSystemModel::changed(const QList<QFileInfo> &list)
{
    bool count_changed = (list.count()==p->list.count());

    for( int i=0 ; i<p->list.count() ; i++ )
    {
        const QFileInfo &file = p->list.at(i);
        if( list.contains(file) )
            continue;

        beginRemoveRows(QModelIndex(), i, i);
        p->list.removeAt(i);
        i--;
        endRemoveRows();
    }

    QList<QFileInfo> temp_list = list;
    for( int i=0 ; i<temp_list.count() ; i++ )
    {
        const QFileInfo &file = temp_list.at(i);
        if( p->list.contains(file) )
            continue;

        temp_list.removeAt(i);
        i--;
    }
    while( p->list != temp_list )
        for( int i=0 ; i<p->list.count() ; i++ )
        {
            const QFileInfo &file = p->list.at(i);
            int nw = temp_list.indexOf(file);
            if( i == nw )
                continue;

            beginMoveRows( QModelIndex(), i, i, QModelIndex(), nw>i?nw+1:nw );
            p->list.move( i, nw );
            endMoveRows();
        }

    for( int i=0 ; i<list.count() ; i++ )
    {
        const QFileInfo &file = list.at(i);
        if( p->list.contains(file) )
            continue;

        beginInsertRows(QModelIndex(), i, i );
        p->list.insert( i, file );
        endInsertRows();
    }

    if(count_changed)
        Q_EMIT countChanged();

    Q_EMIT listChanged();
}

FileSystemModel::~FileSystemModel()
{
    delete p;
}
}
