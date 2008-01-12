#ifndef PLEXY_DESKTOP_WIDGET_H
#define PLEXY_DESKTOP_WIDGET_H	
#include <plexy.h>
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
	class DesktopWidget:public QGraphicsWidget
	{
		Q_OBJECT
	public:
		DesktopWidget(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = Qt::Window);

		virtual ~DesktopWidget();

		virtual void paintBackSide (QPainter * painter,const QRectF& rect);
		virtual void paintViewSide (QPainter * painter,const QRectF& rect);
		virtual void paintDockView (QPainter * painter,const QRectF& rect); 
	protected:
		virtual void paintWindowFrame ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	private:
		class Private ;
		Private * const d;

	
	};
}//

#endif
