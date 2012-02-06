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

#include <QCoreApplication>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QTimeLine>
#include <QTimer>
#include <QDir>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>

#include <QtDebug>

#include "desktopwidget.h"
#include <imagecache.h>
#include <plexy.h>


namespace PlexyDesk
{
class DesktopWidget::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QRectF mBoundingRect;
    QTimeLine *zoomin;
    QTimeLine *zoomout;
    QTimer *spintimer;
    QTimer *mPressHoldTimer;
    State s;
    QPixmap panel;
    QPixmap back;
    QPixmap dock;
    int angle;
    int angleHide;
    QGraphicsProxyWidget *proxyWidget;
    double opacity;
    QRectF saveRect;
    int scale;
    QPointF clickPos;
    bool backdrop;
    bool mEditMode;
    QString mName;

    // image cache
    ImageCache *mCache;
};

DesktopWidget::DesktopWidget(const QRectF &rect, QWidget *widget, QDeclarativeItem *parent) :
    ShaderEffectItem(parent),
    d(new Private)
{
    d->proxyWidget = 0;
    d->mName = QLatin1String ("Widget");
    if (widget) {
        d->proxyWidget = new QGraphicsProxyWidget(this);
        d->proxyWidget->setFocusPolicy(Qt::StrongFocus);
        d->proxyWidget->setWidget(widget);
        d->proxyWidget->show();
    }

    d->mBoundingRect = rect;
    d->mEditMode = false;
    d->backdrop = true;
    d->opacity = 1.0;
    d->saveRect = rect;
    d->s = VIEW;
    d->angle = 0;
    d->angleHide = 0;
    d->scale = 1;


    d->mCache = new ImageCache();
    setDefaultImages();

    setCacheMode(QGraphicsItem::ItemCoordinateCache, d->panel.size());
    setCacheMode(DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setAcceptsHoverEvents(true);

    // zoom in settings
    d->zoomin = new QTimeLine(150, this);
    d->zoomin->setFrameRange(120, 150);
    connect(d->zoomin, SIGNAL(frameChanged(int)),
         this, SLOT(zoomIn(int)));
    connect(d->zoomin, SIGNAL(finished()),
         this, SLOT(zoomDone()));
    // zoom out
    d->zoomout = new QTimeLine(150, this);
    d->zoomout->setFrameRange(0, 150);
    connect(d->zoomout, SIGNAL(frameChanged(int)),
         this, SLOT(zoomOut(int)));
    connect(d->zoomout, SIGNAL(finished()),
         this, SLOT(zoomDone()));
    d->zoomin->start();
    // spin
    d->spintimer = new QTimer(this);
    connect(d->spintimer, SIGNAL(timeout()), this, SLOT(spin()));

    //presshold
    d->mPressHoldTimer = new QTimer(this);
    connect(d->mPressHoldTimer, SIGNAL(timeout()), this, SLOT(pressHoldTimeOut()));
}

DesktopWidget::~DesktopWidget()
{
    delete d;
}

void DesktopWidget::setDefaultImages()
{
    d->panel = d->mCache->get(QLatin1String("panel"));
    d->back = d->mCache->get(QLatin1String("reverse"));
    d->dock = d->mCache->get(QLatin1String("icon"));
}

QRectF DesktopWidget::boundingRect() const
{
    return d->mBoundingRect;
}

QRectF DesktopWidget::rect() const
{
    return d->mBoundingRect;
}

void DesktopWidget::setRect(const QRectF &rect)
{
    d->mBoundingRect = rect;
    prepareGeometryChange();
    resetMatrix();
}

void DesktopWidget::zoomDone()
{
    prepareGeometryChange();
    resetMatrix();
    d->opacity = 1.0;
}

void DesktopWidget::zoomIn(int frame)
{
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.scale(frame / 150.0, frame / 150.0);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->opacity >= 0.0) {
        //d->opacity -= 0.3;
    }
}
void DesktopWidget::zoomOut(int frame)
{
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.scale(1 - frame / 450.0, 1 - frame / 450.0);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->opacity >= 0.0) {
        //d->opacity -= 0.2;
    }
}

void DesktopWidget::setDockBackground(QPixmap img)
{
    d->dock = img;
}

void DesktopWidget::setWidgetBackground(QPixmap img)
{
    d->panel = img;
}

void DesktopWidget::setBacksideBackground(QPixmap img)
{
    d->back = img;
}

void DesktopWidget::setEditMode(const bool &mode)
{
    if (mode) {
        setState(DOCK);
        prepareGeometryChange();
        this->setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        this->setVisible(true);
        d->zoomout->start();
    }

    d->mEditMode = mode;
}

bool DesktopWidget::editMode() const
{
    return d->mEditMode;
}

void DesktopWidget::setIconName(const QString &name)
{
   d->mName = name;
}

void DesktopWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QDeclarativeItem::hoverEnterEvent(event);
}

void DesktopWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
    
void DesktopWidget::setContentRect(const QRectF &rect)
{
    d->saveRect = rect;
}

void DesktopWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) {
        /* We will let the widgets decide what to do with
           left clicks
           */
        QGraphicsItem::mouseDoubleClickEvent(event);
        return;
    }
    if (d->s == DOCK) {
        setState(VIEW);
        prepareGeometryChange();
        this->setRect(d->saveRect);
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
        d->zoomin->start();
    } else {
        setState(DOCK);
        prepareGeometryChange();
        this->setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        this->setVisible(true);
        d->zoomout->start();
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void DesktopWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton && (state() == VIEW || state() == BACK)) {
        d->spintimer->start(18);
        return;
    }

    if (d->mEditMode) {
        d->mPressHoldTimer->stop();
        Q_EMIT close();
        return;
    }

    if (event->buttons() == Qt::LeftButton && state() == DOCK) {
        d->mPressHoldTimer->start(3000);
        qDebug() << Q_FUNC_INFO << "Press start";
        return;
    }
}

void DesktopWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    d->mPressHoldTimer->stop();
    qDebug() << Q_FUNC_INFO << "STOP timer";
    ShaderEffectItem::mouseReleaseEvent(event);
}

void DesktopWidget::spin()
{
    d->angle += 18;
    setCacheMode(ItemCoordinateCache);
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.rotate(d->angle, Qt::XAxis);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->angle >= 180) {
        if (state() == BACK) {
            setState(VIEW);
        } else {
            setState(BACK);
        }
        d->spintimer->stop();
        resetMatrix();
        setCacheMode(DeviceCoordinateCache);
        d->angle = 0;
    }
}

void DesktopWidget::pressHoldTimeOut()
{
    d->mEditMode = true;
    update();
}

void DesktopWidget::drawBackdrop(bool b)
{
    d->backdrop = b;
}

DesktopWidget::State DesktopWidget::state()
{
    return d->s;
}


void DesktopWidget::setState(DesktopWidget::State s)
{
    d->s = s;
}

void DesktopWidget::configState(DesktopWidget::State s)
{
    if (s == d->s) {
        return ;
    }

    resetMatrix();
    prepareGeometryChange();
    
    if (s == DOCK) {
        setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
    } else {
        setRect(QRectF(0, 0, d->panel.width(), d->panel.height()));
    }
    d->s = s;
    if (d->proxyWidget) {
        d->proxyWidget->hide();
    }

    if (d->s == DOCK) {
        setState(VIEW);
        prepareGeometryChange();
        this->setRect(d->saveRect);
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
        d->zoomin->start();
    } else {
        setState(DOCK);
        prepareGeometryChange();
        this->setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        d->zoomout->start();
    }

}

void DesktopWidget::paintBackSide(QPainter *p, const QRectF &rect)
{
    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->back);
    p->restore();
}

void DesktopWidget::paintViewSide(QPainter *p, const QRectF &rect)
{
    if (!d->backdrop)
        return;
    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->panel);
    p->restore();
}

void DesktopWidget::paintDockView(QPainter *p, const QRectF &rect)
{
    if (!d->mEditMode) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->dock);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(8, 5, 64, 64), Qt::AlignCenter, d->mName);
        p->restore();
    }
}

void DesktopWidget::paintEditMode(QPainter *p, const QRectF &rect)
{
    if (d->mEditMode) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->dock);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(8, 5, 64, 64), Qt::AlignCenter, QLatin1String ("Close"));
        p->restore();
    }

}

void DesktopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!painter->isActive())
        return;

    painter->setOpacity(d->opacity);
    painter->setClipRect(option->exposedRect);
    if (d->s == VIEW) {
        paintViewSide(painter, option->exposedRect);
        this->paintExtFace(painter, option, widget);
    } else if (d->s == BACK) {
        paintBackSide(painter, option->exposedRect);
        this->paintExtBackFace(painter, option, widget);
    } else if (d->s == DOCK) {
        paintDockView(painter, option->exposedRect);
        this->paintExtDockFace(painter, option, widget);
    }

    if (d->mEditMode) {
        this->paintEditMode(painter, option->exposedRect);
    }
}

} //namespace PlexyDesk
