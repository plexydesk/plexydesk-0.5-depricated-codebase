//LGPL siraj@kde.org Siraj Razick

#ifndef PLEXY_EXTENION_FACTORY_H
#define PLEXY_EXTENION_FACTORY_H
#include <backdropinterface.h>
#include <pluginloader.h>
#include <widgetinterface.h>
#include <plexy.h>


namespace PlexyDesk
{

template <class T>
class VISIBLE_SYM ExtensionProducer
{
    public:
         T * instance(const QString& name,QObject *obj)
         {
             if ( name.contains("Desktop") == true) {
             return  qobject_cast<T*> (obj);
             }
         }
};

} // namespace PlexyDesk

#endif
