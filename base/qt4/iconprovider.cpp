#include "iconprovider.h"


IconProvider* IconProvider::obj = 0;

IconProvider* IconProvider::instance()
{
    if(!obj) {
     obj = new IconProvider();
    }

    return obj;
}

IconProvider::IconProvider()
{
}

void IconProvider::requestIcon(const QString& name, int size)
{
}

