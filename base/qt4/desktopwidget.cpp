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
#include "desktopwidget.h"
#include <plexy.h>
#include <debug.h>

#include <QCoreApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTimeLine>
#include <QTimer>

#include <QDeclarativeEngine>
#include <QGraphicsObject>
#include <QDeclarativeComponent>
#include <QDir>

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
    QGraphicsObject *qmlChild;
};

DesktopWidget::DesktopWidget(const QRectF &rect, QWidget *widget, QDeclarativeItem *parent) :
    ShaderEffectItem(parent),
    d(new Private)
{
    d->proxyWidget = 0;
    d->qmlChild = 0;
    if (widget) {
        d->proxyWidget = new QGraphicsProxyWidget(this);
        d->proxyWidget->setFocusPolicy(Qt::StrongFocus);
        d->proxyWidget->setWidget(widget);
        d->proxyWidget->show();
    }

    d->backdrop = true;
    d->opacity = 1.0;
    d->saveRect = rect;
    d->s = NORMALSIDE;
    d->angle = 0;
    d->angleHide = 0;
    d->scale = 1;

    d->panel = QPixmap(QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath()  +
         QLatin1String( "/share/plexy/skins/widgets/base-widget/panel.png")));

    d->back = QPixmap(QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath()  +
         QLatin1String("/share/plexy/skins/widgets/base-widget/reverse.png")));

    d->dock = QPixmap(QDir::toNativeSeparators(Config::getInstance()->plexydeskBasePath() +
         QLatin1String("/share/plexy/skins/widgets/base-widget/icon.png")));

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
}

DesktopWidget::~DesktopWidget()
{
    delete d;
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

void DesktopWidget::setDockImage(QPixmap img)
{
    d->dock = img;
}

void DesktopWidget::setFaceImage(QPixmap img)
{
    d->panel = img;
}

void DesktopWidget::setBackFaceImage(QPixmap img)
{
    d->back = img;
}

void DesktopWidget::qmlFromUrl(const QUrl &url)
{
    qDebug() << Q_FUNC_INFO << url << endl;
    if (d->qmlChild) {
        delete d->qmlChild;
    }

    QDeclarativeEngine *engine = QmlEngine();
    QDeclarativeComponent component(engine, url);

    if (!component.isReady()) {
        if (component.isError()) {
            Q_FOREACH(QDeclarativeError error, component.errors()) {
                qDebug() << Q_FUNC_INFO << "Component Error" << error.toString();
            }
        }
        return;
    }
    d->qmlChild =
        qobject_cast<QGraphicsObject *>(component.create(engine->rootContext()));
    QRectF objectRect = d->qmlChild->boundingRect();
    QRectF borderRect(objectRect.x(),
            objectRect.y(),
            objectRect.width(),
            objectRect.height());
    d->saveRect = borderRect;
    setRect(borderRect);
    d->qmlChild->setParentItem(this);
    d->qmlChild->setFlag(QGraphicsItem::ItemIsFocusable, true);
    d->qmlChild->setFlag(QGraphicsItem::ItemIsSelectable, true);
    d->qmlChild->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    update();

    // forward signals
    connect(engine, SIGNAL(quit()), this, SLOT(onQmlQuit()));
}

void DesktopWidget::onQmlQuit()
{
    qDebug() << Q_FUNC_INFO ;
    d->qmlChild->hide();
    scene()->removeItem(d->qmlChild);
    d->qmlChild->setParentItem(0);
    Q_EMIT close();
}

bool DesktopWidget::isQMLWidget() const
{
    if (d->qmlChild) {
        return true;
    }

    return false;
}

void DesktopWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QDeclarativeItem::hoverEnterEvent(event);
}

void DesktopWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
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
        if (d->qmlChild) {
            d->qmlChild->show();
        }
        setState(NORMALSIDE);
        prepareGeometryChange();
        this->setRect(d->saveRect);
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
        d->zoomin->start();
    } else {
        if (d->qmlChild) {
            d->qmlChild->hide();
        }
        setState(DOCK);
        prepareGeometryChange();
        this->setRect(QRectF(0, 0, d->dock.width(), d->dock.height()));
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        d->zoomout->start();
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void DesktopWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (d->qmlChild) {
        QGraphicsItem::mousePressEvent(event);
        return;
    }

    if (event->buttons() == Qt::RightButton && (state() == NORMALSIDE || state() == BACKSIDE)) {
        d->spintimer->start(18);
    }
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
        if (state() == BACKSIDE) {
            setState(NORMALSIDE);
        } else {
            setState(BACKSIDE);
        }
        d->spintimer->stop();
        resetMatrix();
        setCacheMode(DeviceCoordinateCache);
        d->angle = 0;
    }
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
    if (d->qmlChild) {
        return;
    }
    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->panel);
    p->restore();
}

void DesktopWidget::paintDockView(QPainter *p, const QRectF &rect)
{
    p->save();
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->dock);
    p->restore();
}

void DesktopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isObscured())
        return;
    if (!painter->isActive())
        return;

    painter->setOpacity(d->opacity);
    painter->setClipRect(option->exposedRect);
    if (d->s == NORMALSIDE) {
        paintViewSide(painter, option->exposedRect);
        this->paintExtFace(painter, option, widget);
    } else if (d->s == BACKSIDE) {
        paintBackSide(painter, option->exposedRect);
        this->paintExtBackFace(painter, option, widget);
    } else if (d->s == DOCK) {
        paintDockView(painter, option->exposedRect);
        this->paintExtDockFace(painter, option, widget);
    }
}

} //namespace PlexyDesk
