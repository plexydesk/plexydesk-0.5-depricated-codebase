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

ClockWidget::ClockWidget(const QRectF &rect, QWidget *widget)
    : PlexyDesk::DesktopWidget(rect, widget)
{
    shade = 0;

    setPath(QDir::toNativeSeparators (
                PlexyDesk::Config::getInstance()->plexydeskBasePath() + "/share/plexy/skins/default/clock/"));
    drawClockWidget();
}

void ClockWidget::setPath(QString str)
{
    prefix = str + "/";

}

void ClockWidget::drawClockWidget()
{
    _clock_bg = QImage(prefix + "background.png");
    /*gloss stuff*/

    gloss = QImage(prefix + "gloss.png");
    //gloss.convertDepth (32);
    // gloss.setAlphaBuffer (true);

    face = QPixmap(prefix + "face.png");
    thedot = QPixmap(prefix + "thedot.png");
    //date = QPixmap(prefix+"date_field.png");
    /**seconds first **/
    _secs_hand = QPixmap().fromImage(QImage(prefix + "second-hand-long.png"));
    sec_timer = new QTimer(this);
    connect(sec_timer, SIGNAL(timeout()), this, SLOT(drawSeconds()));
    sec_timer->start(1000);
    /** Mins after that **/
    _mins_hand = QPixmap().fromImage(QImage(prefix + "second-hand.png"));
    _hour_hand = QPixmap().fromImage(QImage(prefix + "second-hand.png"));


    /**Createsa a nice Lense**/
    /** Prerender into Qimage to save Processing in Painter event**/
    double rad = ((face.width() / 2)) - 8.0;
    int offset = 28;
    QRect bounds(0, 0, face.width() - offset, face.height() - offset);
    QPainter p;

    lens =
         QImage(QSize(face.width() - offset, face.height() - offset),
         QImage::Format_ARGB32_Premultiplied);

    lens.fill(0);

    p.begin(&lens);

    QRadialGradient gr(rad, rad, rad, 3 * rad / 5, 3 * rad / 5);
    gr.setColorAt(0.0, QColor(255, 255, 255, 191));
    gr.setColorAt(0.2, QColor(255, 255, 231, 191));
    gr.setColorAt(0.9, QColor(150, 150, 200, 65));
    gr.setColorAt(0.95, QColor(0, 0, 0, 0));
    gr.setColorAt(1, QColor(0, 0, 0, 0));
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(gr);
    p.setPen(Qt::NoPen);
    p.drawEllipse(0, 0, bounds.width(), bounds.height());
    p.end();
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

void ClockWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *)
{
    QRectF r = e->exposedRect;
    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(rect(), Qt::transparent);
    p->drawImage(QRect(0, 0, _clock_bg.width(), _clock_bg.height()), _clock_bg);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);
    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    if (shade == 0) {
        p->drawPixmap(_clock_bg.rect(), QPixmap().fromImage(_clock_bg));
        shade = 1;
    }
    p->drawPixmap(QRect(16, 16, face.width(), face.height()), face);
    p->restore();

    /*Draw Hours*/

    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->translate(_clock_bg.width() / 2, _clock_bg.height() / 2);
    p->rotate(_hour);
    p->drawPixmap(QRect
             (-(ceil(double(_hour_hand.width()) / 2)),
             -(_hour_hand.height() - 32), _hour_hand.width(),
             _hour_hand.height()), _hour_hand);
    p->restore();

    /* Draw Mins */
    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->translate(_clock_bg.width() / 2, _clock_bg.height() / 2);
    p->rotate(_mins);
    p->drawPixmap(QRect
             (-(ceil(double(_mins_hand.width()) / 2)),
             -(_mins_hand.height() - 16), _mins_hand.width(),
             _mins_hand.height()), _mins_hand);
    p->restore();

    /*Draw Secs*/
    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->translate(_clock_bg.width() / 2, _clock_bg.width() / 2);
    p->rotate(_secs);
    p->drawPixmap(QRect
             (-(ceil(double(_secs_hand.width()) / 2)),
             -(_secs_hand.height() - 32), _secs_hand.width(),
             _secs_hand.height()), _secs_hand);
    p->restore();

    p->save();

    p->translate(_clock_bg.width() / 2, _clock_bg.width() / 2);
    p->drawPixmap(QRect
             (-(thedot.width() / 2), -(thedot.height() / 2),
             thedot.width(), thedot.height()), QPixmap(thedot));
    p->restore();

    p->drawPixmap(QRect(29, 29, gloss.width(), gloss.height()),
         QPixmap().fromImage(gloss));
    p->drawPixmap(QRect(28, 28, lens.width(), lens.height()),
         QPixmap().fromImage(lens));

    p->drawPixmap(QRect
             ((_clock_bg.width() / 4) * 3, _clock_bg.height() / 2,
             date.width(), date.height()), date);
}

void ClockWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *)
{
    p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    p->setPen(QColor(255, 255, 255));
    p->setFont(QFont("Bitstream Charter", 15));
    p->drawText(QRect(8, 5, 64, 64), Qt::AlignCenter, "Clock");
}

void ClockWidget::paintExtBackFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *)
{
    p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    QRectF r = e->exposedRect;
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
