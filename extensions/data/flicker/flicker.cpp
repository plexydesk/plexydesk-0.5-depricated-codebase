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
         requestID= http->get(QString("/search/?q=%1").arg("allah"));
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
                        images << QString(addr.mid(len,
                                    addr.size()-len).constData());
                    }
                }
                ++i;
            }
         }
              if ( images.size() > 0) {
                   http->setHost("farm3.static.flickr.com");
                   dataID = http->get(images.at(0));
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
