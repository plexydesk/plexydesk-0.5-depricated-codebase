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
#include "example.h"
#include <desktopwidget.h>
#include <plexyconfig.h>



ExampleData::ExampleData(QObject *object)
{
    slideCount = 0;
    currentSlide = 0;
    searchkey = "fresh morning";
    http = new QHttp(this);
    init();
    imageTimer = new QTimer(this);
    connect(imageTimer, SIGNAL(timeout()), this, SLOT(nextImage()));
}

void ExampleData::init()
{

    if (PlexyDesk::Config::getInstance()->proxyOn) {
        QNetworkProxy NtProxy(PlexyDesk::Config::getInstance()->proxyType,
         PlexyDesk::Config::getInstance()->proxyURL,
         PlexyDesk::Config::getInstance()->proxyPort,
         PlexyDesk::Config::getInstance()->proxyUser,
         PlexyDesk::Config::getInstance()->proxyPasswd
         );

        http->setProxy(NtProxy);
        QNetworkProxy::setApplicationProxy(NtProxy);
    }

    connect(http, SIGNAL(requestFinished(int, bool)),
     SLOT(loadCallback(int, bool)));

    http->setHost("www.flickr.com");
    /*note 1 we are saving the request ID , so will call this in loadCallback*/

    requestID = http->get(QString("/search/?w=all&q=%1&m=text").arg(searchkey));
}

ExampleData::~ExampleData()
{
    delete http;
}

void ExampleData::pushData(QVariant &str)
{
    http->abort();
    searchkey = str.toString();
    init();
}

void ExampleData::loadCallback(int id, bool stat)
{
    if (id == requestID) { /* this is a id we know  , and set on init() */
        if (stat) { /*our request had no errors*/
            qDebug() << http->readAll() << endl; //print all the data we got from url
        }
    }

}


#include "example.moc"
