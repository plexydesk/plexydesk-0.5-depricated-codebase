/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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

#include <plexy.h>
#include "youtubewidget.h"
#include <math.h>

#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>

#include <button.h>

namespace PlexyDesk
{

YouTubeWidget::YouTubeWidget(const QRectF &rect) :
     ScrollWidget(rect)
{
    mSearchButton = new PlexyDesk::Button(this);
    mSearchButton->show();
    mSearchButton->setPos(rect.width() - 110 , rect.height() - 40);

    connect(mSearchButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    setContentRect(rect);

    this->enableDefaultBackground(true);
    //this->enableShadow(false);

    mSearchButton->setLable("Search");

    mLineEdit = new PlexyDesk::LineEdit(this);
    mLineEdit->show();
    mLineEdit->setPos(10 , rect.height() - 40);

}

YouTubeWidget::~YouTubeWidget()
{
}

void YouTubeWidget::onDataReady()
{
    QVariant data = utubeEngine->readAll()["data"];

    mView = new QGraphicsTextItem();
    mView->setHtml(data.toString());

    this->addWidget (mView);
    emit dataChanged();
}

void YouTubeWidget::buttonClicked()
{
    this->enableShadow(true);
}

void YouTubeWidget::paintFrontView(QPainter *painter, const QRectF &rect)
{
    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect, 3.5, 3.5);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , rect.height()));


    linearGrad.setColorAt(1, QColor(189, 191, 196));
    linearGrad.setColorAt(0, QColor(255, 255, 255));
    //QColor(189, 191, 196)
    painter->fillPath(backgroundPath, linearGrad);
    QPen pen(QColor(98, 101, 108), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(backgroundPath);
}

} // namespace PlexyDesk
