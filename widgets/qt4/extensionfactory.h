//LGPL siraj@kde.org Siraj Razick

#ifndef PLEXY_EXTENION_FACTORY_H
#define PLEXY_EXTENION_FACTORY_H
#include <backdropinterface.h>
#include <pluginloader.h>
#include <widgetinterface.h>
#include <plexy.h>


namespace PlexyDesk
{
//USAGE OF TEMPLATE DISABLED FOR NOW  

template <class Ext>
class VISIBLE_SYM ExtensionProducer
{
    public:
         AbstractPluginInterface * instance(const QString& name)
         {
             new Ext;
         }
};

/*
    template <class ext>
    AbstractPluginLoader * ExtensionProducer<ext>::createObject()
    {
        return new ext;
    }
    */
/*
    class VISIBLE_SYM ExtensionFactory
    {
        public:
                virtual  BackdropInterface * instance() = 0;
    };
    //PROTOTYPE HACK

    class VISIBLE_SYM WidgetExtensionFactory
    {
        public:
                virtual WidgetInterface * instance() = 0;
    };
*/


} // namespace PlexyDesk

#endif
