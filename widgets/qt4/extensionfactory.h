//LGPL siraj@kde.org Siraj Razick

#ifndef PLEXY_EXTENION_FACTORY_H
#define PLEXY_EXTENION_FACTORY_H
#include  <abstractpluginloder.h>
#include <backdropinterface.h>


namespace PlexyDesk
{
  
template <class Ext>
class ExtensionProducer
    {
    public:
         Ext * createObject()
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

    class VISIBLE_SYM ExtensionFactory
    {
        public:
                virtual  BackdropInterface * instance() = 0;
    };
} // namespace PlexyDesk

#endif
