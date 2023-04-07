#ifndef ACTIVESTARTER_H
#define ACTIVESTARTER_H
#include <QThreadPool>
#include "ActiveRunnable.h"
namespace PH {
template<class OwnerType>
class ActiveStarter{
public:
    static void start(OwnerType*,ActiveRunnableBase* pRunnable){
        pRunnable->setAutoDelete(true);
        QThreadPool::globalInstance()->start(pRunnable);
    }
};
}
#endif // ACTIVESTARTER_H
