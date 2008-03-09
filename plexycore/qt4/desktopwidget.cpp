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
#include "desktopwidget.h"
#include <config.h>


namespace PlexyDesk
{
    class DesktopWidget::Private
    {
        public:
            Private(){}
	    ~ Private(){}
            QTimeLine *zoomin;
            QTimeLine *zoomout;
	    State s;
	    QPixmap panel;
	    QPixmap back;
	    QPixmap dock;
	    int angle;
	    int angleHide;
	    QGraphicsProxyWidget * proxyWidget;	
	    double opacity;
	    QRectF saveRect;
	    int scale;
	    QPointF clickPos;
	    bool backdrop;
   };

    DesktopWidget::DesktopWidget ( const QRectF &rect , QWidget *widget ):
	                           QGraphicsRectItem(rect),d(new Private)
    {	
            d->proxyWidget = 0;
	    if (widget){
	        d->proxyWidget = new QGraphicsProxyWidget (this);
		d->proxyWidget->setFocusPolicy(Qt::StrongFocus);
		d->proxyWidget->setWidget(widget);
		d->proxyWidget->show();
	    }

	    d->backdrop = true;
	    d->opacity = 1.0;
	    d->panel = QPixmap(QString(PLEXPREFIX)+
                               "/share/plexy/skins/widgets/widget01/Panel.png");
	    d->back = QPixmap(QString(PLEXPREFIX)+
                              "/share/plexy/skins/widgets/widget01/reverse.png");
	    d->dock = QPixmap (QString(PLEXPREFIX)+
                               "/share/plexy/skins/widgets/widget01/Icon.png");
	    d->scale = 1;
	    setCacheMode (QGraphicsItem::ItemCoordinateCache,d->panel.size());
	    setCacheMode(DeviceCoordinateCache);
	    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
	    setFlag ( QGraphicsItem::ItemIsMovable , true );
	    setAcceptsHoverEvents ( true );
	    d->saveRect = rect;
	    d->s = NORMALSIDE;
	    d->angle = 0;
	    d->angleHide = 0;

            ///zoom in settings
            d->zoomin = new QTimeLine ( 100 , this );
            d->zoomin->setFrameRange ( 0 , 100 );
            connect ( d->zoomin , SIGNAL ( frameChanged ( int ) ) , 
                      this , SLOT ( zoomIn (int) ) );
            connect ( d->zoomin , SIGNAL ( finished () ) ,
                      this , SLOT ( zoomDone () ) );

            //zoom out 
            d->zoomout = new QTimeLine ( 300 , this );
            d->zoomout->setFrameRange ( 0 , 300 );
            connect ( d->zoomout , SIGNAL ( frameChanged ( int ) ) , 
                      this , SLOT ( zoomOut (int) ) );
            connect ( d->zoomout , SIGNAL ( finished () ) ,
                      this , SLOT ( zoomDone () ) );
    }

    DesktopWidget::~DesktopWidget ( )
    {
        delete d;
    }

    void DesktopWidget::zoomDone ( )
    {
        prepareGeometryChange () ;
        resetMatrix();
        d->opacity = 1.0;
    }

    void DesktopWidget::zoomIn ( int frame )
    {
        QPointF center = boundingRect ().center();
        QTransform mat = QTransform();
        mat.translate (  center.x() ,  center.y() );
        mat.scale ( 1 + frame / 150.0 , 1 + frame / 150.0 );
        mat.translate ( - center.x() ,  - center.y() );
        setTransform (mat);
        if ( d->opacity >= 0.0) {
            d->opacity -= 0.3;
        }
    }
    void DesktopWidget::zoomOut ( int frame )
    {
        QPointF center = boundingRect ().center();
        QTransform mat = QTransform();
        mat.translate (  center.x() ,  center.y() );
        mat.scale ( 1 - frame / 450.0 , 1 - frame / 450.0 );
        mat.translate ( - center.x() ,  - center.y() );
        setTransform (mat);
        if ( d->opacity >= 0.0) {
            d->opacity -= 0.2;
        }
    }

    QRectF DesktopWidget::boundingRect ( ) const
    {
        return rect();
    }	

    void DesktopWidget::setDockImage ( QPixmap img )
    {
        d->dock  = img;
    }

    void DesktopWidget::setFaceImage ( QPixmap img )
    {
        d->panel = img;
    }

    void DesktopWidget::setBackFaceImage(QPixmap img)
   {
       d->back = img;    
   }


   void DesktopWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
   {
       QGraphicsRectItem::hoverEnterEvent(event);
   }


   void  DesktopWidget::mouseMoveEvent ( QGraphicsSceneMouseEvent * event)
   {
	QGraphicsItem::mouseMoveEvent(event);
   }

    void DesktopWidget::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
    {
       if ( d->s == DOCK ) {
           setState( NORMALSIDE );
           prepareGeometryChange ( );
           this->setRect( d->saveRect );
           if ( d->proxyWidget ) {
               d->proxyWidget->show ( );
           }

          d->zoomin->start();
       } else {
           setState( DOCK );
           prepareGeometryChange ( );
           this->setRect ( 0 , 0 , d->dock.width ( ), d->dock.height ( ) );
           if ( d->proxyWidget ) {
               d->proxyWidget->hide ( );
           }
           d->zoomout->start();
       }
   }

    void DesktopWidget::mousePressEvent ( QGraphicsSceneMouseEvent * event )
    {
        if ( event->buttons() == Qt::RightButton) {
         // spin();
        } 
    }


    void DesktopWidget::drawBackdrop(bool b)
    {
        d->backdrop = b;
    }

    DesktopWidget::State DesktopWidget::state()
    {
        return d->s ;
    }


    void DesktopWidget::setState(DesktopWidget::State s)
    {
        d->s = s;
    }


    void DesktopWidget::configState(DesktopWidget::State s)
    {
        resetMatrix();
        prepareGeometryChange ();
        if(s == DOCK){
	    setRect(0,0,d->dock.width(),d->dock.height());
        } else {
	    setRect(0,0,d->panel.width(),d->panel.height());
        }
        d->s = s;
  
       if(d->proxyWidget){  
            d->proxyWidget->hide();
        }
    }


    void DesktopWidget::paintBackSide (QPainter * p,const QRectF& rect)
    {
	p->save();
	p->setOpacity(0.8);
	p->setRenderHints( QPainter::SmoothPixmapTransform);
	p->drawPixmap(QRect(0,0,rect.width(),rect.height()),d->back);
	p->restore();
    }  

    void DesktopWidget::paintViewSide (QPainter * p,const QRectF& rect)
    {
        if(!d->backdrop)
                return;
	p->save();
        p->setOpacity(0.8);
	p->setRenderHints(QPainter::SmoothPixmapTransform );
	p->drawPixmap(QRect(0,0,rect.width(),rect.height()),d->panel);
	p->restore();
    }

    void DesktopWidget::paintDockView (QPainter * p,const QRectF& rect)
   {
       p->save();
       p->setRenderHints(QPainter::SmoothPixmapTransform );
       p->drawPixmap(QRect(0,0,rect.width(),rect.height()),d->dock);
       p->restore();
   }

   void DesktopWidget::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
   {
	
	if (isObscured())
          return;
	painter->setOpacity(d->opacity);
	painter->setClipRect(option->exposedRect);
	if (d->s == NORMALSIDE)
	{
	paintViewSide (painter,option->exposedRect);
	this->paintExtFace(painter,option,widget);
	}
	else if ( d->s == BACKSIDE)
	{
	paintBackSide (painter,option->exposedRect);
	this->paintExtBackFace(painter,option,widget);
	}
	else if ( d->s == DOCK)
	{
	paintDockView (painter,option->exposedRect);
	this->paintExtDockFace(painter,option,widget);
	}
   }

};//namespace PlexyDesk

#include "desktopwidget.moc"
