#include "ListObject.h"
#include <QVariantList>
#include <QDebug>
#include <QDateTime>
namespace kcdz {
class ListObjectPrivate
{
public:
    QVariantList list;
};

ListObject::ListObject(QObject *parent) :
    QObject(parent)
{
    p = new ListObjectPrivate;
}

void ListObject::removeAll(const QVariant &v)
{
    p->list.removeAll( v );
    Q_EMIT countChanged();
}

void ListObject::removeOne(const QVariant &v)
{
    p->list.removeOne( v );
    Q_EMIT countChanged();
}

void ListObject::removeAt(int index)
{
    p->list.removeAt( index );
    Q_EMIT countChanged();
}

QVariant ListObject::takeLast()
{
    if( p->list.isEmpty() )
        return QVariant();

    const QVariant & res = p->list.takeLast();
    Q_EMIT countChanged();

    return res;
}

QVariant ListObject::takeFirst()
{
    if( p->list.isEmpty() )
        return QVariant();

    const QVariant & res = p->list.takeFirst();
    Q_EMIT countChanged();

    return res;
}

QVariant ListObject::takeAt(int index)
{
    const QVariant & res = p->list.takeAt( index );
    Q_EMIT countChanged();

    return res;
}

void ListObject::clear()
{
    p->list.clear();
    Q_EMIT countChanged();
}

QVariant ListObject::last() const
{
    if( p->list.isEmpty() )
        return QVariant();

    return p->list.last();
}

QVariant ListObject::first() const
{
    if( p->list.isEmpty() )
        return QVariant();

    return p->list.first();
}

void ListObject::insert(int index, const QVariant &v)
{
    p->list.insert( index, v );
    Q_EMIT countChanged();
}

void ListObject::append(const QVariant &v)
{
    p->list.append( v );
    Q_EMIT countChanged();
}

void ListObject::prepend(const QVariant &v)
{
    p->list.prepend( v );
    Q_EMIT countChanged();
}

void ListObject::replace(int index, const QVariant &v)
{
    p->list.replace(index, v);
}

void ListObject::swap(int idx0, int idx1)
{
    const auto val_0 = p->list.value(idx0);
    const auto val_1 = p->list.value(idx1);

    p->list[idx0] = val_1;
    p->list[idx1] = val_0;
}

int ListObject::count() const
{
    return p->list.count();
}

bool ListObject::isEmpty() const
{
    return p->list.isEmpty();
}

QVariant ListObject::at(int index) const
{
    if(index == -1)
        return QVariant();

    return p->list.at(index);
}

int ListObject::indexOf(const QVariant &v) const
{
    return p->list.indexOf(v);
}

void ListObject::fromList(const QVariantList &list)
{
    if( p->list == list )
        return;

    p->list = list;
    Q_EMIT countChanged();
}

QVariantList ListObject::toList() const
{
    return p->list;
}

bool ListObject::contains(const QVariant &v) const
{
    return p->list.contains(v);
}

ListObject::~ListObject()
{
    delete p;
}
}
