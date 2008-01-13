#include "videoitem.h"
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
    WidgetItem::WidgetItem(const QRectF &rect, QWidget *widget ):DesktopWidget(rect,widget)
    {

	
    }
    
    WidgetItem::~WidgetItem()
    {

    }

void WidgetItem::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
    {
	
		p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
		
		p->setPen(QColor(215,255,98));
		p->setFont(QFont("Bitstream Charter",20));
		
		if(DesktopWidget::NORMALSIDE)
		{
		p->drawText(QRect(10,10,160,160), Qt::AlignTop ,QDate::longDayName(QDate::currentDate().dayOfWeek ())  ) ;
		p->setFont(QFont("Bitstream Charter",50));
		p->drawText(QRect(40,40,160,160), Qt::AlignTop ,QString("%1").arg(QDate::currentDate().day() )  )  ;
		}
		
    }

void WidgetItem::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{
       
		p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
		
		p->setPen(QColor(215,255,98));
		p->setFont(QFont("Bitstream Charter",12));
		p->drawText(QRect(8,5,64,64), Qt::AlignCenter ,"Calendar" ) ;
}

    QRectF WidgetItem::boundingRect() const
    {
        return rect();
    }

} // namespace PlexyDesk

#include "videoitem.moc"
