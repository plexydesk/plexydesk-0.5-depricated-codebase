#ifndef PLEXY_BASE_PLUGIN_LOADER
#define PLEXY_BASE_PLUGIN_LOADER


#include <plexy.h>
#include <QtCore.h>

namespace PlexyDesk
{
    class VISIBLE_SYM AbstractPluginLoader
    {

        public:
            void init() = 0;

            void getInstance() = 0;

        private:
            AbstractPluginLoader(){}

    };
} // namespace PlexyDesk
