#include "HashObject.h"
#include <QList>
#include <QPair>
#include <QDebug>
#include <QDebug>


namespace kcdz {
class HashObjectPrivate
{
public:
    QMultiHash<QString,QVariant> hash;
};

HashObject::HashObject(QObject *parent) :
    QObject(parent)
{
    p = new HashObjectPrivate;
}


void HashObject::insert(const QString &key, const QVariant &value)
{
    p->hash.replace(key,value);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}


void HashObject::insertMulti(const QString &key, const QVariant &value)
{
    p->hash.insert(key,value);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}



void HashObject::remove(const QString &key)
{
    p->hash.remove(key);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}



void HashObject::remove(const QString &key, const QVariant &value)
{
    p->hash.remove(key,value);
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}



QVariant HashObject::key(const QVariant &value)
{
    return p->hash.key(value);
}


QStringList HashObject::keys(const QVariant &value)
{
    return p->hash.keys(value);
}


QStringList HashObject::uniqueKeys()
{
    return p->hash.uniqueKeys();
}

QStringList HashObject::keys()
{
    return p->hash.keys();
}


QVariant HashObject::value(const QString &key)
{
    return p->hash.value(key);
}


QVariantList HashObject::values(const QString &key)
{
    return p->hash.values(key);
}

QVariantList HashObject::values() const
{
    return p->hash.values();
}


QVariant HashObject::contains(const QString &key)
{
    return p->hash.contains(key);
}

QVariant HashObject::contains(const QString &key, const QVariant &value)
{
    return p->hash.contains(key,value);
}

void HashObject::clear()
{
    if(p->hash.isEmpty())
        return;
    p->hash.clear();
    Q_EMIT countChanged();
    Q_EMIT valuesChanged();
    Q_EMIT keysChanged();
}


int HashObject::count()
{
    return p->hash.count();
}

QVariantMap HashObject::toMap() const
{
    QVariantMap map;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QHashIterator<QString, QVariant> i(p->hash);
#else
    QMultiHashIterator<QString, QVariant> i(p->hash);
#endif
    while(i.hasNext())
    {
        i.next();
        map.insert(i.key(), i.value());
    }
    return map;
}

HashObject::~HashObject()
{
    delete p;
}
}
