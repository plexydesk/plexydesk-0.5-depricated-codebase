#include "flicker.h"
#include <desktopwidget.h>


    
   FlickerData::FlickerData(QObject * object)
    {
	init();
    }
    void  FlickerData::init()
    {
	 http = new QHttp(this);
         connect(http, SIGNAL(requestFinished(int, bool)),
                         SLOT(loadImages(int, bool)));
	 http->setHost("www.flickr.com");    
         requestID= http->get(QString("/search/?w=all&q=%1&m=text").arg("fresh morning"));
    }
    FlickerData::~FlickerData()
    {
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
                  QString hostURL = images.at(0);
                  QString host  (hostURL.mid(7,23));
                  QString fileName (hostURL.mid(24+6,hostURL.length()+1));
                  qDebug()<<"Host name =="<<host<<fileName<<endl;
                  http->setHost(host);
                  dataID = http->get(fileName);
              }

      }  else if (id == dataID) {
          if (http->bytesAvailable() > 0) {
              QByteArray img = http->readAll();
              newWall = QImage(QImage::fromData(img));
              qDebug()<<newWall.size()<<img<<endl;
              if (!newWall.isNull()) {
                  QVariant image(img);
                  emit data(image);
              }
          }
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
