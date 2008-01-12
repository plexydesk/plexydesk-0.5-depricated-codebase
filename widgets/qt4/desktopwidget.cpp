#include "desktopwidget.h"


namespace PlexyDesk
{
class DesktopWidget::Private
{
	public:
		Private(){}
		~ Private(){}
};
	DesktopWidget::DesktopWidget(QGraphicsItem * parent , Qt::WindowFlags wFlags):
	QGraphicsWidget(parent,wFlags),d(new Private)
	{

	}

	DesktopWidget::~DesktopWidget()
	{
	}

void DesktopWidget::paintBackSide (QPainter * painter,const QRectF& rect)
{}

void DesktopWidget::paintViewSide (QPainter * painter,const QRectF& rect)
{}

void DesktopWidget::paintDockView (QPainter * painter,const QRectF& rect)
{}

void  DesktopWidget::paintWindowFrame ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
}

};//

#include "desktopwidget.moc"

