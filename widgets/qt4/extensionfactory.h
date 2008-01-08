//LGPL siraj@kde.org Siraj Razick

#ifndef PLEXY_EXTENION_FACTORY_H
#define PLEXY_EXTENION_FACTORY_H
#include  <abstractpluginloder.h>


namespace PlexyDesk
{
template <class T>
    class ExtensionProducer
    {
    public:
        virtual  AbstractPluginLoader * createObject();
    };

    class ExtenstionFactory
    {
        public:
                virtual  AbstractPluginLoader * instance() = 0;
    };
} // namespace PlexyDesk

#endif
