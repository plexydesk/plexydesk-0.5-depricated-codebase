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

#include <plexy.h>

#include <QNetworkProxy>
#include <QSettings>
#include <QStringList>

namespace PlexyDesk
{
    class PLEXYDESK_EXPORT Config : public QSettings
    {
       Q_OBJECT
    public:
        static Config*  getInstance();
        void read();
        void writeToFile();

        QString MyPictures ;
        QString MyMovies;
        QString CurrentWallpaper;
        bool m_proxyOn;
        int m_proxyPort;
        QNetworkProxy::ProxyType proxyType;
        QString proxyURL;
        QString proxyUser;
        QString proxyPasswd;
        QStringList widgetList;
        bool m_collisionOn;
        QString iconTheme;

     public Q_SLOTS:
        void setWallpaper(const QString& str);
        void addWidget(const QString& widget);

    Q_SIGNALS:
        void configChanged();
        void widgetAdded();
    private:
        Config(const QString & organization, const QString & application = QString(), QObject * parent = 0);
        static Config * config;

        Config() 
        {
            read();
        }

       Config(Config &) {}
       Config& operator=(const Config&);
};
} // namespace PlexyDesk
#endif
