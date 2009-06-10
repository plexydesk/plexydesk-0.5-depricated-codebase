#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QtCore>

class IconProvider : public QObject
{
    Q_OBJECT
public:
     IconProvider* instance();
     void requestIcon(const QString& name, int size);

private:
        IconProvider();
        static IconProvider * obj;


};

#endif // ICONPROVIDER_H
