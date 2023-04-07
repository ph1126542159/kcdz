#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H
#include <QStringList>
#include <QFileInfo>
#include <QObject>
#include "AbstractListModel.h"

namespace kcdz {
class FileSystemModelPrivate;
class FileSystemModel : public AbstractListModel
{
    Q_OBJECT
    Q_ENUMS(SortFlag)
    Q_ENUMS(DataRole)

    Q_PROPERTY(bool showDirs READ showDirs WRITE setShowDirs NOTIFY showDirsChanged)
    Q_PROPERTY(bool showDotAndDotDot READ showDotAndDotDot WRITE setShowDotAndDotDot NOTIFY showDotAndDotDotChanged)
    Q_PROPERTY(bool showDirsFirst READ showDirsFirst WRITE setShowDirsFirst NOTIFY showDirsFirstChanged)
    Q_PROPERTY(bool showFiles READ showFiles WRITE setShowFiles NOTIFY showFilesChanged)
    Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)
    Q_PROPERTY(bool recursive READ recursive WRITE setRecursive NOTIFY recursiveChanged)
    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)
    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(qint32 limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(QString parentFolder READ parentFolder NOTIFY parentFolderChanged)
    Q_PROPERTY(int sortField READ sortField WRITE setSortField NOTIFY sortFieldChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum SortFlag {
        Name,
        Size,
        Date
    };

    enum DataRole {
        FilePath = Qt::UserRole,
        FileUrl,
        FileName,
        FileMime,
        FileSize,
        FileSuffix,
        FileBaseName,
        FileIsDir,
        FileModifiedDate,
        FileCreatedDate
    };

    FileSystemModel(QObject *parent = Q_NULLPTR);
    virtual ~FileSystemModel();

    void setShowDirs(bool stt);
    bool showDirs() const;

    void setShowDotAndDotDot(bool stt);
    bool showDotAndDotDot() const;

    void setShowDirsFirst(bool stt);
    bool showDirsFirst() const;

    void setShowFiles(bool stt);
    bool showFiles() const;

    void setShowHidden(bool stt);
    bool showHidden() const;

    void setNameFilters(const QStringList &list);
    QStringList nameFilters() const;

    void setFolder(const QString &url);
    QString folder() const;

    void setSortField(int field);
    int sortField() const;

    void setRecursive(bool recursive);
    bool recursive() const;

    void setLimit(qint32 limit);
    qint32 limit() const;

    QString parentFolder() const;

#ifndef ASEMAN_OXYGEN_SKIP
    const QFileInfo &id( const QModelIndex &index ) const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QHash<qint32,QByteArray> roleNames() const Q_DECL_OVERRIDE;
#endif
    int count() const;

public Q_SLOTS:
    void refresh();

Q_SIGNALS:
    void countChanged();
    void showDirsChanged();
    void showDotAndDotDotChanged();
    void showDirsFirstChanged();
    void showFilesChanged();
    void showHiddenChanged();
    void nameFiltersChanged();
    void folderChanged();
    void parentFolderChanged();
    void sortFieldChanged();
    void recursiveChanged();
    void limitChanged();
    void listChanged();

private Q_SLOTS:
    void reinit_buffer();

private:
    QList<QFileInfo> ls(const QString &path, qint32 filter, bool recursive, qint32 &limit);
    void changed(const QList<QFileInfo> &list);

private:
    FileSystemModelPrivate *p;
};
}

#endif // FILESYSTEMMODEL_H
