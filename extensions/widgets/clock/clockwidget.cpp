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

#include <plexy.h>
#include "clockwidget.h"
#include <qmatrix.h>
//Added by qt3to4:
#include <QPixmap>
#include <QPaintEvent>
#include <math.h>
#include <QBitmap>
#include <QTimer>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDateTime>
#include <QDir>
#include <plexyconfig.h>
#include <svgprovider.h>

ClockWidget::ClockWidget(const QRectF &rect)
    : PlexyDesk::DesktopWidget(rect)
{
    shade = 0;

    setPath(QDir::toNativeSeparators (
                PlexyDesk::Config::getInstance()->plexydeskBasePath() + "/share/plexy/skins/default/clock"));
    setLabelName ("Clock");
    preRenderClockImages();
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void ClockWidget::setPath(QString str)
{
    prefix = str + "/";
}

void ClockWidget::preRenderClockImages()
{
    PlexyDesk::SvgProvider *svg = new PlexyDesk::SvgProvider();
    QSize clock_face_size (boundingRect().width(), boundingRect().height());

    mClockBackFace = svg->get(QLatin1String("clock#ClockFace"), clock_face_size);

    //TODO Gen pixmaps for clock hands and the clock screw.

    delete svg;
}

void ClockWidget::updateTime(const QVariantMap &data)
{
    time = data["currentTime"].toTime();
    _secs = 6.0 * time.second();
    _mins = 6.0 * time.minute();
    _hour = 30.0 * time.hour();

    update();
}

ClockWidget::~ClockWidget()
{
}

void ClockWidget::drawSeconds()
{
    time = QTime::currentTime();
    _secs = 6.0 * time.second();
    _mins = 6.0 * time.minute();
    _hour = 30.0 * time.hour();

    update();
}

void ClockWidget::paintFrontView(QPainter *p, const QRectF &r)
{
    p->setRenderHint(QPainter::HighQualityAntialiasing);

    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(r, Qt::transparent);
    p->setBackgroundMode(Qt::TransparentMode);

    p->drawPixmap(r.x(), r.y(), r.width(), r.height(), mClockBackFace);

    p->save();
    //draw clock hands
    p->restore();
}

void ClockWidget::paintDockView(QPainter *p, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintDockView(p, rect);
}

void ClockWidget::paintRotatedView(QPainter *p, const QRectF &r)
{
    p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);;
    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(r, Qt::transparent);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);
    p->save();

    p->setPen(QColor(0, 0, 0));
    p->setFont(QFont("Bitstream Charter", 12));
    p->drawText(QRect(10, -10, 200, 140), Qt::AlignLeft | Qt::AlignBottom,
         QDateTime::currentDateTime().toString(QLatin1String("dddd, dd MMMM yyyy")));

    /*Draw Time*/
    p->setPen(QColor(0, 0, 0));
    p->setFont(QFont("Bitstream Charter", 40));
    p->drawText(QRect(0, 0, 160, 160), Qt::AlignCenter,
         QString("%1").arg(QDateTime::currentDateTime().toString(QLatin1String("h:mm"))));
    p->setFont(QFont("Bitstream Charter", 15));
    p->drawText(QRect(0, 0, 190, 180), Qt::AlignCenter | Qt::AlignRight,
         QString("%1").arg(QDateTime::currentDateTime().toString(QLatin1String(":ss"))));
    /* AM/PM */
    p->setFont(QFont("Bitstream Charter", 20));
    p->drawText(QRect(0, 0, 190, 140), Qt::AlignCenter | Qt::AlignRight,
         QString("%1").arg(QDateTime::currentDateTime().toString(QLatin1String("AP"))));
    p->restore();
}
