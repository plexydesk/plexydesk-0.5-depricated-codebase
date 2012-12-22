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

#include "windowbutton.h"
#include <QStyleOptionGraphicsItem>

namespace PlexyDesk
{

class WindowButton::PrivateWindowButton
{
public:
    PrivateWindowButton() {}
    ~PrivateWindowButton() {}

    WindowButtonType mType;
};

WindowButton::WindowButton(QGraphicsObject *parent) :
    PlexyDesk::Button(parent),
    d(new PrivateWindowButton)
{
    d->mType = CLOSE;
}

WindowButton::~WindowButton()
{
    delete d;
}

void WindowButton::setButtonType(WindowButton::WindowButtonType type)
{
    d->mType = type;
}

void WindowButton::paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QRectF rect = option->exposedRect;
    const qreal rad = (rect.width() / 2 ) + 1;
    QRadialGradient gr(rad, rad, rad, 3 * rad / 5, 3 * rad / 5);

    gr.setColorAt(0.0, QColor(255, 255, 255, 191));
    gr.setColorAt(0.2, QColor(255, 255, 127, 191));
    gr.setColorAt(0.9, QColor(150, 150, 200, 63));
    gr.setColorAt(0.95, QColor(0, 0, 0, 127));
    gr.setColorAt(1, QColor(0, 0, 0, 0));

    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath background;
    background.addEllipse(rect);
    painter->fillPath(background, QColor(58, 113, 227));

//    QPen pen = QPen(QColor (111, 111, 111), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

//    painter->setPen(pen);
//    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->drawLine(rect.topLeft(), rect.bottomRight());
//    painter->drawLine(rect.topRight(), rect.bottomLeft());

    painter->setBrush(gr);
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.9);
    painter->drawEllipse(rect);
}

void WindowButton::paintSunkenButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    QRectF rect = option->exposedRect;

    QPen pen = QPen(QColor (0, 111, 111), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->drawLine(rect.topLeft(), rect.bottomRight());

    painter->drawLine(rect.topRight(), rect.bottomLeft());
}

}
