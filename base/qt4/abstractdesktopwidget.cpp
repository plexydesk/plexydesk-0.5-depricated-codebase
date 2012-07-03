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
#include <QtDebug>

#include "abstractdesktopwidget.h"


namespace PlexyDesk
{
class AbstractDesktopWidget::Private
{
public:
    Private() {
    }
    ~Private() {
    }

    State mWidgetState;

    int mScale;
    QString mName;
    float mOpacity;
    bool mEditMode;

    QRectF mDockRect;
    QRectF mContentRect;
    QRectF mBoundingRect;

    QGraphicsProxyWidget *proxyWidget;
};

AbstractDesktopWidget::AbstractDesktopWidget(const QRectF &rect, QWidget *widget, QGraphicsObject *parent) :
    QGraphicsObject(parent),
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

    d->mEditMode = false;
    d->mOpacity = 1.0;
    d->mContentRect = rect;
    d->mWidgetState = VIEW;

    d->mScale = 1.0;
    d->mDockRect = QRectF(0.0, 0.0, 64, 64);
    d->mBoundingRect = rect;

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setAcceptsHoverEvents(true);
}

AbstractDesktopWidget::~AbstractDesktopWidget()
{
    delete d;
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
    update();
}

void AbstractDesktopWidget::setDockRect(const QRectF &rect)
{
    d->mDockRect = rect;
}

QRectF AbstractDesktopWidget::dockRect() const
{
    return d->mDockRect;
}

void AbstractDesktopWidget::setEditMode(const bool &mode)
{
    if (mode) {
        setState(DOCKED);
        prepareGeometryChange();
        this->setRect(d->mDockRect);
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        this->setVisible(true);
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
    QGraphicsObject::hoverEnterEvent(event);
}

void AbstractDesktopWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
}
    
void AbstractDesktopWidget::setContentRect(const QRectF &rect)
{
    d->mContentRect = rect;
}

QRectF AbstractDesktopWidget::contentRect() const
{
    return d->mContentRect;
}

AbstractDesktopWidget::State AbstractDesktopWidget::state()
{
    return d->mWidgetState;
}

void AbstractDesktopWidget::setState(AbstractDesktopWidget::State s)
{
    d->mWidgetState = s;
}

void AbstractDesktopWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!painter->isActive())
        return;

    painter->setOpacity(d->mOpacity);
    painter->setClipRect(option->exposedRect);
    if (d->mWidgetState == VIEW) {
        paintFrontView(painter, option->exposedRect);
    } else if (d->mWidgetState == ROTATED) {
        paintRotatedView(painter, option->exposedRect);
    } else if (d->mWidgetState == DOCKED) {
        paintDockView(painter, option->exposedRect);
    }

    if (d->mEditMode) {
        this->paintEditMode(painter, option->exposedRect);
    }
}

} //namespace PlexyDesk
