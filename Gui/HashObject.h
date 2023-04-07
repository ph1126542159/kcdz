#ifndef HASHOBJECT_H
#define HASHOBJECT_H

#include <QObject>

#include <QVariant>

namespace kcdz {
class HashObjectPrivate;
class  HashObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QVariantList values READ values NOTIFY valuesChanged)
    Q_PROPERTY(QStringList keys READ keys NOTIFY keysChanged)

public:
    HashObject(QObject *parent = Q_NULLPTR);
    virtual ~HashObject();

    QVariantList values() const;

public Q_SLOTS:
    void insert(const QString & key, const QVariant & value );
    void insertMulti(const QString & key, const QVariant & value );
    void remove( const QString & key );
    void remove( const QString & key, const QVariant & value );

    QVariant key( const QVariant & value );
    QStringList keys( const QVariant & value );
    QStringList uniqueKeys();
    QStringList keys();
    QVariant value( const QString & key );
    QVariantList values( const QString & key );

    QVariant contains( const QString & key );
    QVariant contains( const QString & key, const QVariant & value );

    void clear();
    int count();

    QVariantMap toMap() const;

Q_SIGNALS:
    void countChanged();
    void valuesChanged();
    void keysChanged();

private:
    HashObjectPrivate *p;
};
}


#endif // HASHOBJECT_H
