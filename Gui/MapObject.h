#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QObject>

#include <QVariant>


namespace kcdz {
class MapObjectPrivate;
class  MapObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QVariantList values READ values NOTIFY valuesChanged)
    Q_PROPERTY(QStringList keys READ keys NOTIFY keysChanged)

public:
    MapObject(QObject *parent = Q_NULLPTR);
    virtual ~MapObject();

    Q_INVOKABLE void insert(const QString & key, const QVariant & value );
    Q_INVOKABLE void insertMulti(const QString & key, const QVariant & value );
    Q_INVOKABLE void remove( const QString & key );
    Q_INVOKABLE void remove( const QString & key, const QVariant & value );

    Q_INVOKABLE QVariant key( const QVariant & value );
    Q_INVOKABLE QStringList keys( const QVariant & value );
    Q_INVOKABLE QStringList uniqueKeys();
    Q_INVOKABLE QStringList keys();
    Q_INVOKABLE QVariant value( const QString & key );
    Q_INVOKABLE QVariantList values( const QString & key );
    QVariantList values() const;

    Q_INVOKABLE QVariant containt( const QString & key );
    Q_INVOKABLE QVariant containt( const QString & key, const QVariant & value );
    Q_INVOKABLE QVariant contains( const QString & key );
    Q_INVOKABLE QVariant contains( const QString & key, const QVariant & value );

    Q_INVOKABLE void clear();
    Q_INVOKABLE int count();

    Q_INVOKABLE QVariantMap toMap() const;

Q_SIGNALS:
    void countChanged();
    void valuesChanged();
    void keysChanged();

private:
    MapObjectPrivate *p;
};
}
#endif // MAPOBJECT_H
