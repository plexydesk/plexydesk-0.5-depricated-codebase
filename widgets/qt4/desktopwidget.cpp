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
	State s;
	QPixmap panel;
	QPixmap back;
	QPixmap dock;
	int angle;
	QGraphicsProxyWidget * proxyWidget;	
	double opacity;
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
		d->dock = QPixmap ("/usr/share/plexy/skins/widgets/widget01/rIcon.png");

		//setRect(QRect(0,0,d->panel.width(),d->panel.height()));
		setCacheMode (QGraphicsItem::ItemCoordinateCache,d->panel.size());
		setCacheMode(DeviceCoordinateCache);
		setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
		setFlag(QGraphicsItem::ItemIsMovable,true);

		d->s = NORMALSIDE;
		d->angle = 0;
	
		d->timer  = new QTimer(this);
		connect (d->timer , SIGNAL(timeout()),this, SLOT(animate()));
	
		d->hideTimer  = new QTimer(this);
		connect (d->hideTimer , SIGNAL(timeout()),this, SLOT(animateHide()));	
	
	}

	DesktopWidget::~DesktopWidget()
	{
	delete d;
	}
	

void DesktopWidget::spin()
{ 
   d->timer->start(5);
}

void DesktopWidget::hideWidget()
{ 
   d->hideTimer->start(25);
}
void DesktopWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
}

void DesktopWidget::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsRectItem::mouseReleaseEvent(event);
	//d->hideTimer->start(50);
}


QRectF DesktopWidget::boundingRect() const
{
       return rect();//QRect(0,0,d->panel.width(),d->panel.height());	
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
		d->timer->stop();
		d->angle = 0;
		setCacheMode(DeviceCoordinateCache);
		update();
	}

}

void DesktopWidget::animateHide()
{
 if (d->opacity <= 0.0)
   {
	this->hide();
	return;
   }
if(d->proxyWidget)
{
	d->proxyWidget->hide();
}
 d->opacity-=0.1;
update();

}


void DesktopWidget::stateChanged(QTimeLine::State state)
{
}

void DesktopWidget::updateStep(qreal step)
{
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
	p->fillRect(rect,QColor(0,0,255));
	p->restore();
}

void DesktopWidget::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

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

