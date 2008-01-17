
#ifndef PLEXY_CONIFG_LINUX_QT_H
#define PLEXY_CONIFG_LINUX_QT_H

#include <QtCore>
#include <plexy.h>

namespace PlexyDesk
{
    class VISIBLE_SYM Config
    {
        public:
           static Config*  getInstance();
           ///these will be pure virtual on the real plexy desk
           void read();
           void save(){}
           QString MyPictures ;
           QString MyMovies;
           QString CurrentWallpaper;


        private:
           static Config * config;
           Config(){};
           Config(Config &){}
           Config& operator=(const Config&);
    };
} // namespace PlexyDesk



#endif
