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
#include "widgetitem.h"
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
WidgetItem::WidgetItem(const QRectF &rect, QWidget *widget) : DesktopWidget(rect, widget)
{
    setDockImage(QPixmap("/usr/share/plexy/skins/widgets/base-widget/schedule.png"));
}

WidgetItem::~WidgetItem()
{

}

void WidgetItem::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
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

void WidgetItem::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
{

    p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    p->setPen(QColor(255, 255, 255));
    p->setFont(QFont("Bitstream Charter", 15));
    p->drawText(QRect(20, 25, 160, 160), Qt::AlignTop, QDate::shortMonthName(QDate::currentDate().month()));

    p->setPen(QColor(0, 0, 255));
    p->setFont(QFont("Bitstream Charter", 50));
    p->drawText(QRect(20, 40, 160, 160), Qt::AlignTop, QString("%1").arg(QDate::currentDate().day()));

    //  p->drawText(QRect(8,5,64,64), Qt::AlignCenter ,"Calendar" ) ;
}

QRectF WidgetItem::boundingRect() const
{
    return rect();
}

} // namespace PlexyDesk

#include "widgetitem.moc"
