#include "GlobalTranslations.h"
#include <QCoreApplication>
#include <QSet>
namespace kcdz {
class GlobalTranslations::Private
{
public:
    static QSet<GlobalTranslations*> objects;
};

QSet<GlobalTranslations*> GlobalTranslations::Private::objects;

GlobalTranslations::GlobalTranslations(QObject *parent) :
    QObject(parent)
{
    p = new Private;
    p->objects.insert(this);
}

void GlobalTranslations::refresh()
{
    for (GlobalTranslations *o: Private::objects)
        Q_EMIT o->refreshed();
}

GlobalTranslations::~GlobalTranslations()
{
    p->objects.remove(this);
    delete p;
}

}
