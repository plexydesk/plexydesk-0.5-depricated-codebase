/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

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
	   bool proxyOn;
	   int proxyPort;
	   QString proxyURL;
           QString proxyUser;
	   QString proxyPasswd;


        private:
           static Config * config;
           Config(){};
           Config(Config &){}
           Config& operator=(const Config&);
    };
} // namespace PlexyDesk



#endif
