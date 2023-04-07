#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include "AbstractListModel.h"
namespace kcdz {
class ListModel : public AbstractListModel
{
    Q_OBJECT
    class Private;
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QString cachePath READ cachePath WRITE setCachePath NOTIFY cachePathChanged)
    Q_PROPERTY(QVariantList data READ data WRITE setData NOTIFY dataListChanged)

public:
    ListModel(QObject *parent = Q_NULLPTR);
    virtual ~ListModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    QHash<qint32,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int count() const;

    void setCachePath(const QString &path);
    QString cachePath() const;

    QVariantList data() const;
    void setData(const QVariantList &data);

Q_SIGNALS:
    void countChanged();
    void cachePathChanged();
    void listChanged();
    void dataListChanged();

public Q_SLOTS:
    void insert(int row, const QVariantMap &map);
    void append(const QVariantMap &map);
    void remove(int row);
    void move(int from, int to);
    void clear();
    void change(const QVariantList &list);

protected:
    virtual void changed(const QList<QVariantMap> &list);

private:
    void loadCache();
    void saveCache();

private:
    Private *p;
};
}


#endif // LISTMODEL_H
