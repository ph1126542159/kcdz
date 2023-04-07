#ifndef GLOBALTRANSLATIONS_H
#define GLOBALTRANSLATIONS_H

#include <QObject>
#include "Core/Singleholder.h"
namespace kcdz {
class  GlobalTranslations : public QObject,public PH::SingleHolder<GlobalTranslations>
{
    Q_OBJECT
    Q_PROPERTY(QString refresher READ refresher NOTIFY refreshed)
    class Private;
    friend class TranslationManager;
    friend class PH::SingleHolder<GlobalTranslations>;
public:
    ~GlobalTranslations();

    QString refresher() const { return QString(); }

Q_SIGNALS:
    void refreshed();

private:
    static void refresh();
    GlobalTranslations(QObject *parent = Q_NULLPTR);
    Private *p;
};
}

#endif // GLOBALTRANSLATIONS_H
