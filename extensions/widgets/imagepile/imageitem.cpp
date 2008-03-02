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
#include "imageitem.h"
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
    ImagePileWidget::ImagePileWidget(const QRectF &rect, QWidget *widget ):DesktopWidget(rect,widget)
    {
    }
    
    ImagePileWidget::~ImagePileWidget()
    {
    }

    void ImagePileWidget::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
    {
    }
    
void ImagePileWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
    {
		
    }

void ImagePileWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
    {
    }

    QRectF ImagePileWidget::boundingRect() const
    {
        return rect();
    }

} // namespace PlexyDesk

#include "imageitem.moc"
