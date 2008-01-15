#ifndef PLEXY_DESKTOP_WIDGET_H
#define PLEXY_DESKTOP_WIDGET_H	
#include <plexy.h>
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
	class DesktopWidget:public QObject, public QGraphicsRectItem
	{
		Q_OBJECT
	public:
		typedef enum {	DOCK,
				NORMALSIDE,
				BACKSIDE} State;

		DesktopWidget(const QRectF &rect, QWidget *embeddedWidget = 0);

		virtual ~DesktopWidget();

		virtual void paintBackSide (QPainter * painter,const QRectF& rect);
		virtual void paintViewSide (QPainter * painter,const QRectF& rect);
		virtual void paintDockView (QPainter * painter,const QRectF& rect); 
		    
                        void drawBackdrop(bool);
		        void setState(State s);
		        State state(); 
                        void setDockImage(QPixmap);
                        void setFaceImage(QPixmap);
                        void setBackFaceImage(QPixmap );

		virtual QRectF boundingRect() const;
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
		virtual void paintExtFace(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0){}
		virtual void paintExtBackFace(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0){}

		virtual void paintExtDockFace(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0){}
		//virtual void setOpacity(double op);
		//virtual double opacity();
		
	
	public slots:
				void updateStep(int);
				void stateChanged(QTimeLine::State);
				void animate();
				void hideWidget();
				virtual void spin();
				//virtual void animateHide();
				
	protected:
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0); 
		virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	private:
		class Private ;
		Private * const d;

	
	};
}//

#endif
