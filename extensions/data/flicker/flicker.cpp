#include "flicker.h"
#include <desktopwidget.h>


    
   FlickerData::FlickerData(QObject * object):QObject(object)
    {
//	init();
    }
    void  FlickerData::init()
    {
	 http = new QHttp(this);
         connect(http, SIGNAL(requestFinished(int, bool)),
                         SLOT(loadImages(int, bool)));
	 http->setHost("http://www.flickr.com");    
         requestID= http->get(QString("/search/?q=%1").arg("allah"));
    }
    FlickerData::~FlickerData()
    {
    }
   
   void FlickerData::loadImages(int id, bool stat)
   {
	qDebug()<<id<<stat<<endl;
	if (requestID == id){

        }
	
	if (dataID == id){
        	
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
