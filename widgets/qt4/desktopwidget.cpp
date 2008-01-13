#include "desktopwidget.h"


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
		d->opacity = 1.0;
		//d->hidding = false;
		d->panel = QPixmap("/usr/share/plexy/skins/widgets/widget01/Panel.png");
		d->back = QPixmap("/usr/share/plexy/skins/widgets/widget01/reverse.png");
		d->dock = QPixmap ("/usr/share/plexy/skins/widgets/widget01/Icon.png");
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


void DesktopWidget::spin()
{ 
   d->timer->start(5);
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
	QPointF center = d->clickPos;
	int x_ = center.x() - d->dock.width()/2;
	int y_ = center.y() -d->dock.height()/2 ;
	setState(DOCK);
	d->opacity = 1.0;
	update();
	setPos(mapToScene(QPoint(x_,y_))); 
	qDebug()<<d->clickPos<<endl;
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
    //resetMatrix();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
   mat.scale(1 - frame / 150.0, 1 - frame / 150.0);
  // mat.rotate(frame/8.0,Qt::XAxis);
  //  rotate(frame / 8.0);
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

void DesktopWidget::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsRectItem::mouseReleaseEvent(event);
}


void DesktopWidget::animate()
{

    d->angle += 15;
    setCacheMode(NoCache);
    QPointF center = this->boundingRect().center();
    //resetMatrix();
    QTransform mat = QTransform();//this->transform();
    mat.translate(center.x(),center.y());
    mat.rotate(d->angle,Qt::YAxis);
    mat.translate(-center.x(),-center.y());
    this->setTransform(mat);

	if ( d->angle == 360)
	{

	if(d->s ==  NORMALSIDE)
	{
		setState(BACKSIDE);
	}
	else if ( d->s == BACKSIDE)
	{
		setState(NORMALSIDE);
	}
		d->hideTimer->stop();
		d->angleHide = 0;
		setCacheMode(DeviceCoordinateCache);
		update();
	}

}







void DesktopWidget::setState(DesktopWidget::State s)
{
d->s = s;
}
void DesktopWidget::paintBackSide (QPainter * p,const QRectF& rect)
{
	p->save();
	p->setRenderHints( QPainter::SmoothPixmapTransform);
	p->drawPixmap(QRect(0,0,rect.width(),rect.height()),d->back);
	p->restore();
}

void DesktopWidget::paintViewSide (QPainter * p,const QRectF& rect)
{
	p->save();
	p->setRenderHints(QPainter::SmoothPixmapTransform );
	p->drawPixmap(QRect(0,0,rect.width(),rect.height()),d->panel);
	//p->fillRect(rect,QColor(0,222,0));
	p->restore();
}

void DesktopWidget::paintDockView (QPainter * p,const QRectF& rect)
{
	p->save();
	p->setRenderHints(QPainter::SmoothPixmapTransform );
	p->drawPixmap(QRect(0,0,rect.width(),rect.height()),d->dock);
	//p->fillRect(rect,QColor(0,222,0));
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
	painter->setRenderHints(QPainter::TextAntialiasing);
	painter->setPen(Qt::white);
     	painter->setFont(QFont("Arial", 10));
	painter->drawText(QRectF(rect().width()-45,rect().height()-45,30,30), Qt::AlignCenter, "+");
}

};//

#include "desktopwidget.moc"

