/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
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
#include "welcomewidget.h"
#include <math.h>

#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>
#include <QPushButton>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <imagecache.h>

#include "welcomeitem.h"
#include "frameitem.h"

class WelcomeWidget::Private {
public:
    Private() {
    }
    ~Private() {
    }
    //refactor
    QRectF clip;
    QRectF view;


    int shade;
    int moveY;
    QImage m_bg;
    QImage mItem_bg;

    QString prefix;
    QPoint clickPos;

    PlexyDesk::DataPlugin *utubeEngine;

    QMap<QString, QVariant> mVariantMap;
    QVariantMap mMap;
    PlexyDesk::DataPlugin *plugin;

    WelcomeItem *icon1;
    WelcomeItem *icon2;
    WelcomeItem *icon3;
};

WelcomeWidget::WelcomeWidget(const QRectF &rect, QWidget *widget) :
    PlexyDesk::DesktopWidget(rect, widget), d(new Private)
{

    QRectF _rect(0.0, 0.0, 128.0, 128.0);
    d->icon1 = new WelcomeItem(QRect(0, 0, 128, 256), this);
    d->icon1->setName("");
    d->icon1->setIcon(fromSvg(_rect, "home"));
    d->icon1->setPos(60, 70);

    d->icon2 = new WelcomeItem(QRect(0, 0, 128, 256), this);
    d->icon2->setName("");
    d->icon2->setIcon(fromSvg(_rect, "notifications"));
    d->icon2->setPos(220, 70);

    d->icon3 = new WelcomeItem(QRect(0, 0, 128, 256), this);
    d->icon3->setName("");
    d->icon3->setIcon(fromSvg(_rect, "relationships"));
    d->icon3->setPos(380, 70);
}

WelcomeWidget::~WelcomeWidget()
{
    delete d;
}

QPixmap WelcomeWidget::fromSvg(QRectF _rect, const QString &file)
{
    PlexyDesk::ImageCache *cache = PlexyDesk::ImageCache::instance();
    QImage pixmap(_rect.width(), _rect.height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter;
    painter.begin(&pixmap);
    painter.setBackgroundMode(Qt::TransparentMode);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(_rect, Qt::transparent);
    painter.save();
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    cache->drawSvg(&painter, _rect, file);
    painter.restore();
    painter.end();

    return QPixmap::fromImage(pixmap);
}
void WelcomeWidget::showLogin()
{
    //startSpin();
}

void WelcomeWidget::onDataReady()
{
}

void WelcomeWidget::fadeIn()
{

}

void WelcomeWidget::paintBackSide(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintViewSide(painter, rect);
    d->icon1->setVisible(false);
    d->icon2->setVisible(false);
    d->icon3->setVisible(false);
}
void WelcomeWidget::paintViewSide(QPainter *painter, const QRectF &rect)
{
    d->icon1->setVisible(true);
    d->icon2->setVisible(true);
    d->icon3->setVisible(true);
}


void WelcomeWidget::fadeOut()
{
}

void WelcomeWidget::data(QVariantMap &data)
{
    qDebug() << Q_FUNC_INFO << data["data"];
}
