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
#include <svgprovider.h>
#include <plexy.h>


namespace PlexyDesk
{
class AbstractDesktopWidget::Private
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
    SvgProvider *mSvgRender;
};

AbstractDesktopWidget::AbstractDesktopWidget(const QRectF &rect, QWidget *widget, QDeclarativeItem *parent) :
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


    d->mSvgRender = new SvgProvider();
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

AbstractDesktopWidget::~AbstractDesktopWidget()
{
    delete d;
}

void AbstractDesktopWidget::setDefaultImages()
{
    d->dock = enableDefaultBackground(72, 72);
    d->panel = enableDefaultBackground(boundingRect().width(),
            boundingRect().height());
    d->back = d->panel;
}

QPixmap AbstractDesktopWidget::enableDefaultBackground(int w, int h)
{
    if (w == 0 || h == 0) {
        return QPixmap();
    }

    /* widget top */
    QRect dock_rect (0, 0, w, h);
    QRect dock_topleft(0, 0, 10, 10);
    QRect dock_topright((dock_rect.width() - 10), 0, 10, 10);
    QRect dock_top(10, 0, (dock_rect.width() - 20), 10);

    QRect dock_right((dock_rect.width() - 10), 10, 10, (dock_rect.height() - 10));

    QRect dock_bottomright((dock_rect.width() - 10), (dock_rect.height() - 10), 10, 10);
    QRect dock_bottom(10, (dock_rect.height() - 10), (dock_rect.width() - 20), 10);
    QRect dock_bottomleft(0, (dock_rect.height() - 10), 10, 10);

    QRect dock_left (0, 10, 10, (dock_rect.height() - 20));

    QImage canvas (dock_rect.height(), dock_rect.width(), QImage::Format_ARGB32_Premultiplied);

    QPainter svg_painter(&canvas);
    svg_painter.setCompositionMode(QPainter::CompositionMode_Source);
    svg_painter.setBackgroundMode(Qt::TransparentMode);
    svg_painter.setRenderHint(QPainter::SmoothPixmapTransform);
    svg_painter.fillRect(dock_rect, Qt::transparent);

    svg_painter.save();
    svg_painter.setPen (Qt::black);
    svg_painter.drawRect(dock_rect);
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_rect,
            d->mSvgRender->get(QLatin1String("background#center"), dock_rect.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_topleft,
            d->mSvgRender->get(QLatin1String("background#topleft"), dock_topleft.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_topright,
            d->mSvgRender->get(QLatin1String("background#topright"), dock_topleft.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_top,
            d->mSvgRender->get(QLatin1String("background#top"), dock_top.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_right,
            d->mSvgRender->get(QLatin1String("background#right"), dock_right.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_bottomright,
            d->mSvgRender->get(QLatin1String("background#bottomright"), dock_bottomright.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_bottom,
            d->mSvgRender->get(QLatin1String("background#bottom"), dock_bottom.size()));
    svg_painter.restore();

    svg_painter.save(); svg_painter.drawPixmap(dock_bottomleft,
            d->mSvgRender->get(QLatin1String("background#bottomleft"), dock_bottomleft.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_left,
            d->mSvgRender->get(QLatin1String("background#left"), dock_left.size()));
    svg_painter.restore();

    svg_painter.end();

    return QPixmap::fromImage(canvas);
}

QRectF AbstractDesktopWidget::boundingRect() const
{
    return d->mBoundingRect;
}

QRectF AbstractDesktopWidget::rect() const
{
    return d->mBoundingRect;
}

void AbstractDesktopWidget::setRect(const QRectF &rect)
{
    d->mBoundingRect = rect;
    prepareGeometryChange();
    resetMatrix();

    setDefaultImages();
    update();
}

void AbstractDesktopWidget::zoomDone()
{
    prepareGeometryChange();
    resetMatrix();
    d->opacity = 1.0;
}

void AbstractDesktopWidget::zoomIn(int frame)
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
void AbstractDesktopWidget::zoomOut(int frame)
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

void AbstractDesktopWidget::setDockBackground(QPixmap img)
{
    d->dock = img;
}

void AbstractDesktopWidget::setWidgetBackground(QPixmap img)
{
    d->panel = img;
}

void AbstractDesktopWidget::setBacksideBackground(QPixmap img)
{
    d->back = img;
}

void AbstractDesktopWidget::setEditMode(const bool &mode)
{
    if (mode) {
        setState(DOCKED);
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

bool AbstractDesktopWidget::editMode() const
{
    return d->mEditMode;
}

void AbstractDesktopWidget::setIconName(const QString &name)
{
   d->mName = name;
}

void AbstractDesktopWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QDeclarativeItem::hoverEnterEvent(event);
}

void AbstractDesktopWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
    
void AbstractDesktopWidget::setContentRect(const QRectF &rect)
{
    d->saveRect = rect;
}

void AbstractDesktopWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) {
        /* We will let the widgets decide what to do with
           left clicks
           */
        QGraphicsItem::mouseDoubleClickEvent(event);
        return;
    }
    if (d->s == DOCKED) {
        setState(VIEW);
        prepareGeometryChange();
        this->setRect(d->saveRect);
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
        d->zoomin->start();
    } else {
        setState(DOCKED);
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

void AbstractDesktopWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton && (state() == VIEW || state() == ROTATED)) {
        d->spintimer->start(36);
        return;
    }

    if (d->mEditMode) {
        d->mPressHoldTimer->stop();
        Q_EMIT closed();
        return;
    }

    if (event->buttons() == Qt::LeftButton && state() == DOCKED) {
        d->mPressHoldTimer->start(3000);
        qDebug() << Q_FUNC_INFO << "Press start";
        return;
    }
}

void AbstractDesktopWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    d->mPressHoldTimer->stop();
    qDebug() << Q_FUNC_INFO << "STOP timer";
    ShaderEffectItem::mouseReleaseEvent(event);
}

void AbstractDesktopWidget::spin()
{
    d->angle += 18;
    setCacheMode(ItemCoordinateCache);
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.rotate(d->angle, Qt::YAxis);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->angle >= 180) {
        if (state() == ROTATED) {
            setState(VIEW);
        } else {
            setState(ROTATED);
        }
        d->spintimer->stop();
        resetMatrix();
        setCacheMode(DeviceCoordinateCache);
        d->angle = 0;
    }
}

void AbstractDesktopWidget::pressHoldTimeOut()
{
    d->mEditMode = true;
    update();
}

void AbstractDesktopWidget::enableDefaultBackground(bool b)
{
    d->backdrop = b;
}

AbstractDesktopWidget::State AbstractDesktopWidget::state()
{
    return d->s;
}


void AbstractDesktopWidget::setState(AbstractDesktopWidget::State s)
{
    d->s = s;
}

void AbstractDesktopWidget::configState(AbstractDesktopWidget::State s)
{
    if (s == d->s) {
        return ;
    }

    resetMatrix();
    prepareGeometryChange();
    
    if (s == DOCKED) {
        setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
    } else {
        setRect(QRectF(0, 0, d->panel.width(), d->panel.height()));
    }
    d->s = s;
    if (d->proxyWidget) {
        d->proxyWidget->hide();
    }

    if (d->s == DOCKED) {
        setState(VIEW);
        prepareGeometryChange();
        this->setRect(d->saveRect);
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
        d->zoomin->start();
    } else {
        setState(DOCKED);
        prepareGeometryChange();
        this->setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        d->zoomout->start();
    }

}

void AbstractDesktopWidget::paintRotatedView(QPainter *p, const QRectF &rect)
{
    if (!d->backdrop) {
        return;
    }

    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->back);
    p->restore();
}

void AbstractDesktopWidget::paintFrontView(QPainter *p, const QRectF &rect)
{
    if (!d->backdrop)
        return;
    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, d->panel.width(), d->panel.height()), d->panel);
    p->restore();
}

void AbstractDesktopWidget::paintDockView(QPainter *p, const QRectF &rect)
{
    if (!d->mEditMode) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->dock);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(0, 0,72, 72), Qt::AlignCenter, d->mName);
        p->restore();
    }
}

void AbstractDesktopWidget::paintEditMode(QPainter *p, const QRectF &rect)
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

void AbstractDesktopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!painter->isActive())
        return;

    painter->setOpacity(d->opacity);
    painter->setClipRect(option->exposedRect);
    if (d->s == VIEW) {
        paintFrontView(painter, option->exposedRect);
        this->paintExtFace(painter, option, widget);
    } else if (d->s == ROTATED) {
        paintRotatedView(painter, option->exposedRect);
        this->paintExtBackFace(painter, option, widget);
    } else if (d->s == DOCKED) {
        paintDockView(painter, option->exposedRect);
        this->paintExtDockFace(painter, option, widget);
    }

    if (d->mEditMode) {
        this->paintEditMode(painter, option->exposedRect);
    }
}

} //namespace PlexyDesk
