#include "plexyconfig.h"

namespace PlexyDesk
{
    Config * Config::config = 0;

    Config * Config::getInstance()
    {
        if ( config == 0) {
         return  new Config();
        }else {
            return config;
        }
    }

}


