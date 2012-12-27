#ifndef SOCIALACCESSMANAGER_H
#define SOCIALACCESSMANAGER_H

#include <QObject>
#include <plexy.h>

#include "plexydeskuicore_global.h"

namespace PlexyDesk {

class PLEXYDESKUICORE_EXPORT SocialAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit SocialAccessManager(QObject *parent = 0);

Q_SIGNALS:
    void finished(SocialAccessManager *manager);

public Q_SLOTS:
    //todo
};
} //PlexyDesk
#endif // SOCIALACCESSMANAGER_H
