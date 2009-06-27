#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QtCore>
#include <config.h>
#include <plexy.h>
#include <iconjob.h>

namespace PlexyDesk
{

class VISIBLE_SYM IconProvider : public QObject
{
    Q_OBJECT
public:
    IconProvider ();
    IconJobPtr requestIcon(const QString& name, const QString&);
private:
    class Private;
    Private * const d;
};

typedef QSharedPointer<IconProvider> IconProviderPtr;
}

#endif // ICONPROVIDER_H
