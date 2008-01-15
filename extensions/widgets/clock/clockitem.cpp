#include "clockitem.h"
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
    ClockItem::ClockItem(const QRectF &rect, QWidget *widget ):DesktopWidget(rect,widget)
    {
        setDockImage(QPixmap("/usr/share/plexy/skins/widgets/widget01/schedulepng.png"));
    }
    
    ClockItem::~ClockItem()
    {

    }

void ClockItem::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
    {
/*	
		p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
		
		p->setPen(QColor(215,255,98));
		p->setFont(QFont("Bitstream Charter",20));
		
		if(DesktopWidget::NORMALSIDE)
		{
		p->drawText(QRect(10,10,160,160), Qt::AlignTop ,QDate::longDayName(QDate::currentDate().dayOfWeek ())  ) ;
		p->setFont(QFont("Bitstream Charter",50));
		p->drawText(QRect(40,40,160,160), Qt::AlignTop ,QString("%1").arg(QDate::currentDate().day() )  )  ;
		}
                */
		
    }

void ClockItem::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{
       
		p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
		
		p->setPen(QColor(255,255,255));
		p->setFont(QFont("Bitstream Charter",15));
		p->drawText(QRect(20,25,160,160), Qt::AlignTop ,QDate::shortMonthName(QDate::currentDate().month() ) ) ;

		p->setPen(QColor(0,0,255));
		p->setFont(QFont("Bitstream Charter",50));
		p->drawText(QRect(20,40,160,160), Qt::AlignTop ,QString("%1").arg(QDate::currentDate().day() )  )  ;
	
	//	p->drawText(QRect(8,5,64,64), Qt::AlignCenter ,"Calendar" ) ;
}

    QRectF ClockItem::boundingRect() const
    {
        return rect();
    }

} // namespace PlexyDesk

#include "clockitem.moc"
