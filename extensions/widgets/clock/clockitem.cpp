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
#include "clockitem.h"
#include <config.h>
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
ClockItem::ClockItem(const QRectF &rect, QWidget *widget ):DesktopWidget(rect,widget)
{
    setDockImage(QPixmap(applicationDirPath() + "/share/plexy/skins/widgets/widget01/schedulepng.png"));
}

ClockItem::~ClockItem()
{

}

void ClockItem::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{
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
}

QRectF ClockItem::boundingRect() const
{
    return rect();
}

} // namespace PlexyDesk

#include "clockitem.moc"
