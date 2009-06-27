/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  TODO : Add collabora header
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
#include "desktopview.h"
#include <QtGui>
#include <QGLWidget>
#include <qwebviewitem.h>
#include <pictureflow.h>

class  WebView::Private
{
public:
        Private() {}
        ~Private() {}
        QList<QGraphicsItem*> weblist;
        QPixmap reflect;
        QImage mirror;
        PictureFlow * coverflow;
};

bool getLessThanWidget(const QGraphicsItem* it1, const QGraphicsItem* it2)
{
       return it1->zValue() < it2->zValue();
}

void WebView::addWebPage(QGraphicsItem * item)
{
   if (!item) {
       return ;
   }
   d->weblist.append(item);
   QWebViewItem * page = (QWebViewItem*) item;
   connect(page, SIGNAL(previewUpdate()), this, SLOT(updateReflect()));
}

void WebView::updateReflect()
{
  if ( d->weblist.at(0) ) {
      QWebViewItem * page  = (QWebViewItem*) d->weblist.at(0);
      page->show();
      //d->reflect = page->getPreview(false);
      //d->coverflow->addSlide(d->reflect);
      /*
      QImage preview  = QImage(d->reflect.size(), QImage::Format_ARGB32_Premultiplied);
      QPainter p1(&preview);
      p1.fillRect(page->boundingRect(), Qt::transparent);
      p1.drawPixmap(QRect(0,0,d->reflect.width(), d->reflect.height()) , d->reflect );
      p1.end();

      d->mirror = preview.mirrored(0,1);
      QPainter p(&d->mirror);
      QPoint x, y;
      y.setY(d->mirror.height());
      QLinearGradient grad(x, y);
      grad.setColorAt(0, QColor(0, 0, 0,95));
      grad.setColorAt(1, Qt::transparent);
      p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
      p.fillRect(QRect(0, 0, d->mirror.width(), d->reflect.height()), grad);
      p.end();


      invalidateScene(QRect(0,480, 800, 480));
      updateSceneRect (QRect(0,480, 800, 480));
      */
  }
}

WebView::WebView ( QGraphicsScene * scene, QWidget * parent ):QGraphicsView(scene,parent),d(new Private)
{
  //      setWindowFlags(Qt::FramelessWindowHint);
//        setAttribute(Qt::WA_ContentsPropagated );
//        setViewport(new QGLWidget(QGLFormat(QGL::DirectRendering | QGL::SampleBuffers)));
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
        setFrameStyle(QFrame::NoFrame);
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setCacheMode(QGraphicsView::CacheBackground);
        d->coverflow = new PictureFlow(this);
        d->coverflow->resize(800,480);
        d->coverflow->hide();
       // d->coverflow->setBackgroundColor(Qt::white);
        
        d->reflect = QPixmap(); 
}

WebView::~WebView()
{
        delete d;
}

void WebView::backgroundChanged()
{
        invalidateScene();
        scene()->update();
}

void WebView::addExtension(const QString& name)
{
}

void WebView::addCoreExtension(const QString& name)
{

}

void WebView::drawBackground ( QPainter * painter, const QRectF & rect )
{
        painter->save();
        painter->setClipRect(rect);
     //   painter->fillRect(rect, QColor(0,0,0));
      //  painter->drawImage(QRect(0, 480,800,480),d->mirror);
        painter->restore();
}

void WebView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}
void WebView::zoomView(int value)
{
} 

void WebView::setTopMostWidget(const QPoint &pt)
{
        int i = 0;
        QGraphicsItem *clickedItem = scene()->itemAt(pt);
        if (clickedItem == 0)
                return;

        QList<QGraphicsItem *> itemsList = scene()->items();
        qStableSort(itemsList.begin(), itemsList.end(), getLessThanWidget);

        clickedItem->setZValue(itemsList.size());

        foreach(QGraphicsItem* item, itemsList) {
                if (item == clickedItem)
                        continue;

                item->setZValue(i);
                i++;
        }

        clickedItem->update();
}

#include "desktopview.moc"
