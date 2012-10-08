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
    setLabelName ("Clock");
    preRenderClockImages();
}

void ClockWidget::preRenderClockImages()
{
    PlexyDesk::SvgProvider *svg = new PlexyDesk::SvgProvider();

    mClockBackFace = svg->get(QLatin1String("clock#ClockFace"));
    mClockSecondHand = svg->get(QLatin1String("clock#SecondHand"));
    mClockMinuteHand = svg->get(QLatin1String("clock#MinuteHand"));
    mClockHourHand = svg->get(QLatin1String("clock#HourHand"));
    mClockScrew = svg->get(QLatin1String("clock#HandCenterScrew"));
    mClockGlass = svg->get(QLatin1String("clock#Glass"));

    setContentRect(mClockBackFace.rect());

    delete svg;
}

void ClockWidget::updateTime(const QVariantMap &data)
{
    QTime time = data["currentTime"].toTime();
    mSecondValue = 6.0 * time.second();
    mMinutesValue = 6.0 * time.minute();
    mHourValue = 30.0 * time.hour();

    update();
}

ClockWidget::~ClockWidget()
{
}

void ClockWidget::paintFrontView(QPainter *p, const QRectF &r)
{
    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(r, Qt::transparent);
    p->setBackgroundMode(Qt::TransparentMode);

    p->drawPixmap(r.x(), r.y(), r.width(), r.height(), mClockBackFace);
    float scaleFactorHorizontal = contentRect().width() / boundingRect().width();
    float scaleFactorVerticle = contentRect().height() / boundingRect().height();

    /* Draw Minute Hand */
    p->save();
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->translate( r.x() + (r.width() / 2), r.y() + (r.height() / 2));
    p->rotate(180 + mMinutesValue);
    p->drawPixmap(-(mClockMinuteHand.width() / 2) , 0, mClockMinuteHand.width() / scaleFactorHorizontal, mClockMinuteHand.height() / scaleFactorVerticle, mClockMinuteHand);
    p->restore();

    /* Draw Hour Hand */
    p->save();
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->translate( r.x() + (r.width() / 2), r.y() + (r.height() / 2));
    p->rotate(180 + mHourValue);
    p->drawPixmap(-(mClockHourHand.width() / 2) , 0, mClockHourHand.width() / scaleFactorHorizontal, mClockHourHand.height() / scaleFactorVerticle, mClockHourHand);
    p->setOpacity(0.2);
    p->drawPixmap(-(mClockHourHand.width() / 2) + 2 , 2, mClockHourHand.width() / scaleFactorHorizontal, mClockHourHand.height() / scaleFactorVerticle, mClockHourHand);
    p->restore();

    /* Draw Second Hand */
    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->translate( r.x() + (r.width() / 2), r.y() + (r.height() / 2));
    p->rotate(180 + mSecondValue);
    p->drawPixmap( -(mClockSecondHand.width() / 2) , 0 , mClockSecondHand.width() / scaleFactorHorizontal, mClockSecondHand.height() / scaleFactorVerticle, mClockSecondHand);
    p->setOpacity(0.3);
    p->drawPixmap( -(mClockSecondHand.width() / 2) + 2 , 2 , mClockSecondHand.width() / scaleFactorHorizontal, mClockSecondHand.height() / scaleFactorVerticle, mClockSecondHand);
    p->restore();

    /* Draw the Clock Screw  */
    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->translate(r.x() + (r.width() / 2) , r.y() + (r.height() / 2));
    p->drawPixmap( -(mClockScrew.width() / 2) , -(mClockScrew.height() / 2), mClockScrew.width() / scaleFactorHorizontal, mClockScrew.height() / scaleFactorVerticle, mClockScrew);
    p->restore();

    /* Draw Glass */
    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->translate(r.x() + (r.width() / 2) , r.y() + (r.height() / 2));
    p->drawPixmap( -(mClockGlass.width() / 2) , -(mClockGlass.height() / 2), mClockGlass.width() / scaleFactorHorizontal, mClockGlass.height() / scaleFactorVerticle, mClockGlass);
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
