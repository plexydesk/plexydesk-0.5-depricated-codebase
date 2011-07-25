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

#include <QStyleOptionGraphicsItem>
#include <QPainter>

namespace PlexyDesk
{
ImagePileWidget::ImagePileWidget(const QRectF &rect, QWidget *widget) : DesktopWidget(rect, widget)
{
    setDockImage(QPixmap(applicationDirPath() + "/share/plexy/skins/widgets/base-widget/pila.png"));
    cover = QImage(200, 200, QImage::Format_ARGB32_Premultiplied);
}

ImagePileWidget::~ImagePileWidget()
{
}


void ImagePileWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
{

}

void ImagePileWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
{
    QRectF r = e->exposedRect;
    p->drawImage(QRectF(20, 20, r.width() - 40, r.height() - 40), cover);
}

QRectF ImagePileWidget::boundingRect() const
{
    return rect();
}

void ImagePileWidget::setCoverPic(QImage img)
{
    cover = img;
}

} // namespace PlexyDesk
