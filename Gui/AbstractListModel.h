#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H

#include <QAbstractListModel>
namespace kcdz {
class  AbstractListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    AbstractListModel(QObject *parent = Q_NULLPTR);
    virtual ~AbstractListModel();

    Q_INVOKABLE QStringList roles() const;

public Q_SLOTS:
    QVariant get(int index, int role) const;
    QVariant get(int index, const QString &roleName) const;
    QVariantMap get(int index) const;
};
}


#endif // ABSTRACTLISTMODEL_H
