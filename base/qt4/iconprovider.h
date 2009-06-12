#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QtCore>
#include <config.h>
#include <plexy.h>


class Requests
{
    public:
     Requests(){}

    QString name;
    QString size;
    bool valid;
    uint id;
};

//typedef struct request Request;

namespace PlexyDesk {

class VISIBLE_SYM IconProvider : public QObject
{
    Q_OBJECT
public:
     IconProvider ();
     uint requestIcon(const QString& name, const QString&);
     QStringList getSubDir(const QString& path);
     QPixmap getIcon(uint id);
public Q_SLOTS:
     void loadIcons();
     Q_SIGNALS:
     void requestAdded();
     void iconPixmap(const QPixmap&, uint);
private:
        class Private;
        Private * const d;

};

typedef QSharedPointer<IconProvider> IconProviderPtr;
}

#endif // ICONPROVIDER_H
