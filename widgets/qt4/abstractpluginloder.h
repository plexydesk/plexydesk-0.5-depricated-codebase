#ifndef PLEXY_BASE_PLUGIN_LOADER
#define PLEXY_BASE_PLUGIN_LOADER


#include <plexy.h>
#include <QtCore>

namespace PlexyDesk
{
    class VISIBLE_SYM AbstractPluginLoader
    {

        public:
           virtual void init() = 0;


    };
} // namespace PlexyDesk

#endif
