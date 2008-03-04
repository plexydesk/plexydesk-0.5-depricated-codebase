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
	QTimer * timer;
	QTimer * hideTimer;
	QTimer * showTimer;
	QTimeLine * timeLine;
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
	//bool hindding;
};
	DesktopWidget::DesktopWidget(const QRectF &rect, QWidget *widget):
	QGraphicsRectItem(rect),d(new Private)
	{	
		d->proxyWidget = 0;
		if (widget)
		{
			d->proxyWidget = new QGraphicsProxyWidget (this);
			d->proxyWidget->setFocusPolicy(Qt::StrongFocus);
			d->proxyWidget->setWidget(widget);
			//d->proxyWidget->setGeometry(re);	
			d->proxyWidget->show();
		}
		d->backdrop = true;
		d->opacity = 1.0;
		//d->hidding = false;
		d->panel = QPixmap(QString(PLEXPREFIX)+"/share/plexy/skins/widgets/widget01/Panel.png");
		d->back = QPixmap(QString(PLEXPREFIX)+"/share/plexy/skins/widgets/widget01/reverse.png");
		d->dock = QPixmap (QString(PLEXPREFIX)+"/share/plexy/skins/widgets/widget01/Icon.png");
		d->scale = 1;
		//setRect(QRect(0,0,d->panel.width(),d->panel.height()));
		setCacheMode (QGraphicsItem::ItemCoordinateCache,d->panel.size());
		setCacheMode(DeviceCoordinateCache);
		setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
		setFlag(QGraphicsItem::ItemIsMovable,true);
		setAcceptsHoverEvents(true);
		d->saveRect = rect;
		d->s = NORMALSIDE;
		d->angle = 0;
		d->angleHide = 0;
		d->timer  = new QTimer(this);
		connect (d->timer , SIGNAL(timeout()),this, SLOT(animate()));

		d->hideTimer  = new QTimer(this);
		connect (d->hideTimer , SIGNAL(timeout()),this, SLOT(animateHide()));	
	
			d->timeLine = new QTimeLine();
			d->timeLine->setDuration(300);
			d->timeLine->setFrameRange(0, 300);
    			connect(d->timeLine, SIGNAL(frameChanged(int)), this, SLOT(updateStep(int)));
    			connect(d->timeLine, SIGNAL(stateChanged ( QTimeLine::State)), this, 
    			SLOT( stateChanged(QTimeLine::State) ));
	
	}

DesktopWidget::~DesktopWidget()
{
	delete d;
}


QRectF DesktopWidget::boundingRect() const
{
       return rect();//QRect(0,0,d->panel.width(),d->panel.height());	
}	

void DesktopWidget::setDockImage(QPixmap img)
{
    d->dock  = img;
}

void DesktopWidget::setFaceImage(QPixmap img)
{
d->panel = img;
}

void DesktopWidget::setBackFaceImage(QPixmap img)
{
    d->back = img;
}

void DesktopWidget::spin()
{ 
   d->timer->start(10);
}

void DesktopWidget::hideWidget()
{ 
        d->timeLine->setDirection(QTimeLine::Forward);
        if (d->timeLine->state() == QTimeLine::NotRunning)
        {
                        d->timeLine->start();
        }
}

void DesktopWidget::stateChanged(QTimeLine::State state)
{
        if(state == QTimeLine::NotRunning)
        {
        resetMatrix();
	prepareGeometryChange ();
	setRect(0,0,d->dock.width(),d->dock.height());
	QPointF center = rect().center();//d->clickPos;
	int x_ = center.x();// - d->dock.width()/2;
	int y_ = center.y();// -d->dock.height()/2 ;
	setState(DOCK);
	d->opacity = 1.0;
	update();
	setPos(mapToScene(QPoint(x_,y_))); 
	setCacheMode(DeviceCoordinateCache);
	        if(d->proxyWidget)
	        {
	                d->proxyWidget->hide();
	        }
        }
}


void DesktopWidget::updateStep(int frame)
{


    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.scale(1 - frame / 450.0, 1 - frame / 450.0);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
       
                if( d->opacity >= 0.0)
                {
                         d->opacity -= 0.2;
                }
                update();
}


void DesktopWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
QGraphicsRectItem::hoverEnterEvent(event);
}


void  DesktopWidget::mouseMoveEvent ( QGraphicsSceneMouseEvent * event)
{
/*
	Config::getInstance()->read();
	
	if (!Config::getInstance()->collitionOn){
	 QGraphicsItem::mouseMoveEvent(event);
	 return;
       }else {
	QList<QGraphicsItem*>  list = collidingItems();
	if(list.count() > 0){
		foreach(QGraphicsItem *item,list) { 
                 if(item == this) {
                 break;
                 } else{
                               DesktopWidget  * wigy = (DesktopWidget*)item;
				if( wigy->state() == DOCK)
				wigy->spin();
                }
	    }
        }
     }
     */
	QGraphicsItem::mouseMoveEvent(event);
}

void DesktopWidget::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{

d->clickPos = event->pos();

if( d->s != DOCK)
{
if (d->proxyWidget )
{
        d->proxyWidget->hide();
}
hideWidget();
}
else
{
	d->hideTimer->stop();
	resetMatrix();
	prepareGeometryChange ();
	setRect(d->saveRect);
	setState(NORMALSIDE);
	d->opacity = 1.0;
	if (d->proxyWidget )
        {
        d->proxyWidget->show();
        }
	update();
	setCacheMode(DeviceCoordinateCache);
}

QGraphicsRectItem::mouseDoubleClickEvent(event);
}

void DesktopWidget::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
        if ( event->buttons() == Qt::RightButton) {
            spin();
        } else {

        qDebug()<<"Not doing any thing "<<endl;
        }

}


void DesktopWidget::animate()
{

    d->angle += 36/2;
    //setCacheMode(NoCache);
    setCacheMode(ItemCoordinateCache);
    QPointF center = this->boundingRect().center();
//    resetMatrix();
    QTransform mat = QTransform();//this->transform();
    mat.translate(center.x(),center.y());
 //   mat.scale(0.5,0.5);
    mat.rotate(d->angle,Qt::YAxis);
   // mat.scale(2.0,2.0);
    mat.translate(-center.x(),-center.y());
    this->setTransform(mat);

	if ( d->angle == 180)
	{

	if(d->s ==  NORMALSIDE)
	{
		setState(BACKSIDE);
	}
	else if ( d->s == BACKSIDE)
	{
		setState(NORMALSIDE);
	} else {

        }
		d->timer->stop();
		d->angle = 0;
		resetMatrix();
		setCacheMode(DeviceCoordinateCache);
                d->opacity = 1.0;
		update();
		return;
	}

        d->opacity -= 0.2;


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
//hack
    d->s = s;
}


void DesktopWidget::configState(DesktopWidget::State s)
{
        //resetMatrix();
	prepareGeometryChange ();
	if(s == DOCK){
	setRect(0,0,d->dock.width(),d->dock.height());
        }
	else {
	setRect(0,0,d->panel.width(),d->panel.height());
       }
	
		d->s = s;
//	QPointF center = rect().center();//d->clickPos;
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
//	painter->save();
//	QTransform trans= QTransform ();
//	trans.rotate(180,Qt::YAxis);
        //painter->setTransform(trans,true);;
	this->paintExtBackFace(painter,option,widget);
//	painter->restore();
	}
	else if ( d->s == DOCK)
	{
	paintDockView (painter,option->exposedRect);
	this->paintExtDockFace(painter,option,widget);
	}
        /*
	painter->setRenderHints(QPainter::TextAntialiasing);
	painter->setPen(Qt::white);
     	painter->setFont(QFont("Bitstream Charter", 10));
	painter->drawText(QRectF(rect().width()-25,rect().height()-25,30,30), Qt::AlignCenter, "+");
        */
}

};//

#include "desktopwidget.moc"

