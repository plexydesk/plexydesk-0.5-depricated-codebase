/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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

#include <plexy.h>
#include "youtubewidget.h"
#include <math.h>

#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>

namespace PlexyDesk
{

YouTubeWidget::YouTubeWidget (const QRectF &rect, QWidget *widget):
DesktopWidget(rect,widget)
{
  shade = 0;
  moveY = 0;
  setPath(applicationDirPath() +"/theme/skins/default/widget/default/youtube/");
  setDockImage(QPixmap(prefix + "icon.png"));

  PlexyDesk::DataInterface * utubeEngine = (PlexyDesk::DataInterface*)
          PlexyDesk::PluginLoader::getInstance()->instance("utubeengine");

  if (utubeEngine) {
      connect(utubeEngine,SIGNAL(data(QVariant&)),this,SLOT(data(QVariant&)));
      //connect(utubeEngine,SIGNAL(success()),this,SLOT(drawItems()));
  }else {
      qDebug("DataSource Was Null");
  }

  connect(this,SIGNAL(dataChanged()),this,SLOT(drawItems()));
  drawWidget();

  clip = QRectF(10, 10, rect.width()-40, rect.height()-20);
  view = QRectF(0, 0, rect.width()-80, 0);
}

YouTubeWidget::~YouTubeWidget ()
{}

void YouTubeWidget::setPath(QString str)
{
  prefix  = str+"/";
}

void YouTubeWidget::data(QVariant& data)
{
    mVariantMap = data.toMap();

    VideoEntity videoentity;
    videoentity.title = (mVariantMap["title"]).toString();
    videoentity.link = (mVariantMap["link"]).toString();
    videoentity.desc = (mVariantMap["description"]).toString();
    videoentity.thumb = (mVariantMap["thumb"]).toString();

    mVideos.append(videoentity);
    view.setHeight(mVideos.size()*mItem_bg.height());
    emit dataChanged();
}

void
        YouTubeWidget::drawWidget()
{
   m_bg = QImage (prefix + "bacground.png");

   mItem_bg = QImage (prefix + "videotag.png");

}

void
        YouTubeWidget::drawItems()
{
    update ();
}

void YouTubeWidget::wheelEvent(QGraphicsSceneWheelEvent * event)
{
         update();
         int numDegrees = event->delta() /2 ;
         int numSteps = numDegrees / 15;
         int expectedY = view.y();

         if(expectedY == 0 && numSteps > 0 ) {
            return;
        }
         if( abs(view.y()) > (view.bottom() - clip.height()) && numSteps < 0 )
             return;
         view.setY(view.y()+numSteps);


         qDebug()<<view.bottom()<< ":"<<view.y()<<endl;

}


void YouTubeWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *)
{
    QRectF r  = e->exposedRect;
    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(rect(), Qt::transparent);
    p->drawImage (QRect(r.x(),r.y(),m_bg.width(),m_bg.height()),m_bg);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);
    p->save ();
    p->setRenderHint (QPainter::SmoothPixmapTransform);
    p->setClipRect(clip);
    int vidcount = 0;
    for(int i = 5 ; i < mVideos.size()* mItem_bg.height(); ) {
        if(vidcount>= mVideos.size())
            break;
        VideoEntity videoentity;
        videoentity = (VideoEntity) mVideos.at(vidcount++);

        p->setPen( QColor(255,255,255) );
        p->setFont( QFont("Bitstream Charter",10,QFont::Bold) );
        p->drawText(40,view.y()+i+15,300,30,Qt::AlignLeft,QString(videoentity.title) );

        p->setFont( QFont("Bitstream Charter",8) );
        p->drawText(40,view.y()+i+40,250,100,Qt::AlignLeft|Qt::TextWordWrap,QString(videoentity.desc) );

         p->drawImage( QRect(clip.x()+1,view.y()+i,mItem_bg.width(),mItem_bg.height()),mItem_bg);
         i += mItem_bg.height();
  }
    p->restore();
    /*
  QRectF r  = e->exposedRect;
  p->setCompositionMode(QPainter::CompositionMode_Source);
  p->fillRect(rect(), Qt::transparent);

  p->drawImage (QRect(r.x(),r.y(),m_bg.width(),m_bg.height()),m_bg);
  p->setCompositionMode(QPainter::CompositionMode_SourceOver);
  p->setBackgroundMode(Qt::TransparentMode);
  p->save ();
  p->setRenderHint (QPainter::SmoothPixmapTransform);
  if (shade == 0)
    {
      p->drawPixmap (m_bg.rect (), QPixmap ().fromImage(m_bg));
      shade = 1;
    }

  p->restore ();
  p->save();
  p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
  QRect cliper (0,20,r.width(),r.height()-60);
  p->setClipRect(cliper);
  if(mVideos.size()>0){
    int texty=moveY; int tagy=moveY+30;

    //FIXME:
    //use foreach
    for (int i = 0; i < mVideos.size(); ++i) {
        VideoEntity videoentity;
        videoentity = (VideoEntity) mVideos.at(i);

        p->drawImage( QRect(r.x()+10,tagy,mItem_bg.width(),mItem_bg.height()),mItem_bg);

        p->setPen( QColor(255,255,255) );
        p->setFont( QFont("Bitstream Charter",10,QFont::Bold) );
        p->drawText(40,texty+60,300,30,Qt::AlignLeft,QString(videoentity.title) );

        p->setPen( QColor(255,255,255) );
        p->setFont( QFont("Bitstream Charter",9) );
        p->drawText(40,texty+90,250,100,Qt::AlignLeft|Qt::TextWordWrap,QString(videoentity.desc) );

        texty += mItem_bg.height(); tagy += mItem_bg.height();
    }
  }
  p->restore();
  */
}


void
YouTubeWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e
, QWidget *)
{
    p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);

    p->setPen(QColor(255,255,255));
    p->setFont(QFont("Bitstream Charter",15));
    p->drawText(QRect(8,5,64,64), Qt::AlignCenter ,"YouTube" ) ;
}

} // namespace PlexyDesk
