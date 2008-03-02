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
#include "flicker.h"
#include <desktopwidget.h>
#include <plexyconfig.h>


    
   FlickerData::FlickerData(QObject * object)
    {
        slideCount = 0;
        currentSlide = 0;
	init();
        imageTimer = new QTimer(this);
        connect(imageTimer,SIGNAL(timeout()),this,SLOT(nextImage()));
    }
    void  FlickerData::init()
    {
	 http = new QHttp(this);
	
	 PlexyDesk::Config::getInstance()->read();
	 if(PlexyDesk::Config::getInstance()->proxyOn == true){ 
	 QNetworkProxy NtProxy(PlexyDesk::Config::getInstance()->proxyType,
		               PlexyDesk::Config::getInstance()->proxyURL,
                               PlexyDesk::Config::getInstance()->proxyPort,
                               PlexyDesk::Config::getInstance()->proxyUser,
                               PlexyDesk::Config::getInstance()->proxyPasswd
			      );

              http->setProxy(NtProxy);
	      QNetworkProxy::setApplicationProxy(NtProxy);
	qDebug()<<"FlickerData::init()"<<"Proxy state"<<PlexyDesk::Config::getInstance()->proxyOn<<endl;
	}
	
         connect(http, SIGNAL(requestFinished(int, bool)),
                         SLOT(loadImages(int, bool)));
	 http->setHost("www.flickr.com");    
         requestID= http->get(QString("/search/?w=all&q=%1&m=text").arg("fresh morning"));
    }
    FlickerData::~FlickerData()
    {
    }
   void FlickerData::nextImage()
   {

       QString hostURL = images.at(currentSlide);
       QString host  (hostURL.mid(7,23));
       QString fileName (hostURL.mid(24+6,hostURL.length()+1));

       http->setHost(host);
       dataID = http->get(fileName);
       currentSlide++;
       if (currentSlide > slideCount-1) {
            currentSlide = 0;
            imageTimer->stop();
       }
   }
   void FlickerData::loadImages(int id, bool stat)
   {
     if (id == requestID) {
        if (http->bytesAvailable() > 0) {
            QByteArray ba = http->readAll();
            const char *data = ba.constData();
            const int len = 30;
            int i = 0;
            while (i < ba.size()-5) {
                if (data[i] == '.') {
                    if (data[i+1] == 'j' && data[i+2] == 'p'
                        && data[i+3] == 'g' && data[i+4] == '\"') {
                        int j = i;
                        while (j > 0 && data[j] != '\"') --j;
                        QByteArray addr(ba.mid(j+1, i-j+3));
                        char farmID = addr.at(11);
                        QByteArray serverID (addr.mid(30,4));
                        addr = addr.replace(QByteArray("_m"),QByteArray("_o"));
                        // qDebug()<<addr<<farmID<<serverID<<endl;
                        // images << QString(addr.mid(len,
                        //addr.size()-len).constData());
                        images<< QString(addr);
                    }
                }
                ++i;
            }
         }
              if ( images.size() > 0) {
                  slideCount = images.count();
                  imageTimer->start(5000);
              }

      }  
     if (id >  requestID ) {
          if (http->bytesAvailable() > 0) {
              QByteArray img = http->readAll();
              newWall = QImage(QImage::fromData(img));
              if (!newWall.isNull()) {
                  QVariant image(img);
                  emit data(image);
              }
          }
      }

    if (id == slideCount -1) {
        imageTimer->start(5000);
    }

   }

   QGraphicsItem * FlickerData::item()
   {
   }

   void FlickerData::render(QPainter *p,QRectF r)
   {
   }

Q_EXPORT_PLUGIN2(FlickerData,FlickerData)
#include "flicker.moc"
