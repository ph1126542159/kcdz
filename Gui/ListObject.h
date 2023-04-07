#ifndef LISTOBJECT_H
#define LISTOBJECT_H

#include <QObject>
#include <QVariant>
namespace kcdz {
class ListObjectPrivate;
class  ListObject : public QObject
{
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QVariantList list READ toList WRITE fromList NOTIFY countChanged)

    Q_OBJECT
public:
    ListObject(QObject *parent = Q_NULLPTR);
    virtual ~ListObject();

public Q_SLOTS:
    void removeAll( const QVariant & v );
    void removeOne( const QVariant & v );
    void removeAt( int index );
    QVariant takeLast();
    QVariant takeFirst();
    QVariant takeAt( int index );

    void clear();

    QVariant last() const;
    QVariant first() const;

    void insert( int index, const QVariant & v );
    void append( const QVariant & v );
    void prepend( const QVariant & v );

    void replace(int index, const QVariant &v);
    void swap(int idx0, int idx1);

    int count() const;
    bool isEmpty() const;

    QVariant at( int index ) const;
    int indexOf( const QVariant & v ) const;

    void fromList( const QVariantList & list );
    QVariantList toList() const;

    bool contains( const QVariant & v ) const;

Q_SIGNALS:
    void countChanged();

private:
    ListObjectPrivate *p;
};
}

#endif // LISTOBJECT_H
