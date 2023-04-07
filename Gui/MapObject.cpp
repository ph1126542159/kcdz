#include "MapObject.h"
#include <QList>
#include <QPair>
#include <QDebug>

namespace kcdz {
class MapObjectPrivate
{
public:
    QMultiMap<QString,QVariant> map;
};

MapObject::MapObject(QObject *parent) :
    QObject(parent)
{
    p = new MapObjectPrivate;
}

void MapObject::insert(const QString &key, const QVariant &value)
{
    p->map.replace(key,value);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}

void MapObject::insertMulti(const QString &key, const QVariant &value)
{
    p->map.insert(key,value);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}

void MapObject::remove(const QString &key)
{
    p->map.remove(key);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}

void MapObject::remove(const QString &key, const QVariant &value)
{
    p->map.remove(key,value);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}

QVariant MapObject::key(const QVariant &value)
{
    return p->map.key(value);
}

QStringList MapObject::keys(const QVariant &value)
{
    return p->map.keys(value);
}

QStringList MapObject::uniqueKeys()
{
    return p->map.uniqueKeys();
}

QStringList MapObject::keys()
{
    return p->map.keys();
}

QVariant MapObject::value(const QString &key)
{
    return p->map.value(key);
}

QVariantList MapObject::values(const QString &key)
{
    return p->map.values(key);
}

QVariantList MapObject::values() const
{
    return p->map.values();
}

QVariant MapObject::containt(const QString &key)
{
    return contains(key);
}

QVariant MapObject::containt(const QString &key, const QVariant &value)
{
    return contains(key, value);
}

QVariant MapObject::contains(const QString &key)
{
    return p->map.contains(key);
}

QVariant MapObject::contains(const QString &key, const QVariant &value)
{
    return p->map.contains(key,value);
}

void MapObject::clear()
{
    if(p->map.isEmpty())
        return;
    p->map.clear();
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}

int MapObject::count()
{
    return p->map.count();
}

QVariantMap MapObject::toMap() const
{
    QVariantMap map;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QMapIterator<QString, QVariant> i(p->map);
#else
    QMultiMapIterator<QString, QVariant> i(p->map);
#endif
    while(i.hasNext())
    {
        i.next();
        map.insert(i.key(), i.value());
    }
    return map;
}

MapObject::~MapObject()
{
    delete p;
}

}
