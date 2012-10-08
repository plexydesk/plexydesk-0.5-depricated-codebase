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

#include "controllerinterface.h"
#include "abstractdesktopwidget.h"

/**
 * \class PlexyDesk::AbstractDesktopWidget
 *
 * \ingroup PlexyDesk
 *
 * \brief Base class for Visual Desktop Extensions
 * PlexyDesk::AbstractDesktopWidget defines all the common features of a visual desktop extension.
 * When writing visual extensions, the user must inherit from this class. So that it behaves the
 * same way.
 */

/**
 * \fn PlexyDesk::AbstractDesktopWidget::boundingRect()
 * \breif Returns the Bounding rectangle of the Widget Content
 *
 * \paragraph This method returns the bounding Rectangle of the Widget Contents
 * it returns the user specificed content rectangle (PlexyDesk::AbstractDesktopWidget::setContentRect())
 * And does not automatically calculate or adjust with the content changes, The size of the content must be
 * manually set if the content changes and this widget needs to adjust to that size.
 *
 * @returns The bounding Rectangle for this widget
 * \sa PlexyDesk::AbstractDesktopWidget::setContentRect()
 * \sa PlexyDesk::AbstractDesktopWidget::setDockRect()
 */

/**
* \fn PlexyDesk::AbstractDesktopWidget::setContentRect()
*\brief Set the content Rectangle of the Widget
*
* \paragraph Sets the content rectangle of the widget
* in situations where the widget can't
* find out by it self. For instance widets which changes
* it's content during runtime or chiild QML widgets.
* Use it only if you are changing the widget size.
*
* \param  rect The Content bounding box area
*/

/**
* \fn PlexyDesk::AbstractDesktopWidget::setRect()
*\brief Set the bounding Rectangle of the Widget
*
* \paragraph Sets the bounding rectangle of the widget
* in situations where the widget rectangle property needs
* to be set.
*
* \param  rect The Content bounding box area
*/

/**
  *\fn PlexyDesk::AbstractDesktopWidget::setChildWidetVisibility()
  *
  * \brief Hides the child items of this widget
  *
  * \param show Decides the visibility of the child widgets, set true to show the child elements
  * false to hide them.
  *
  */

/** PlexyDesk::AbstractDesktopWidget::setLabel
  * \fn
  * \brief Sets a label for the widget
  * \param name Name of the icon
  *
  * Label to display when the widget is in dock mode
  */


namespace PlexyDesk
{
class AbstractDesktopWidget::PrivateAbstractDesktopWidget
{
public:
    PrivateAbstractDesktopWidget() : mController(0) {
    }
    ~PrivateAbstractDesktopWidget() {
    }

    State mWidgetState;

    int mScale;
    QString mName;
    float mOpacity;
    float mAngle;
    bool mEditMode;

    QRectF mDockRect;
    QRectF mContentRect;
    QRectF mBoundingRect;

    ControllerInterface *mController;
};


AbstractDesktopWidget::AbstractDesktopWidget(const QRectF &rect, QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d(new PrivateAbstractDesktopWidget)
{
    d->mName = QLatin1String ("Widget");

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

void AbstractDesktopWidget::setDockRect(const QRectF &rect)
{
    d->mDockRect = rect;
}

QRectF AbstractDesktopWidget::dockRect() const
{
    return d->mDockRect;
}

float AbstractDesktopWidget::rotation() const
{
    return d->mAngle;
}

void AbstractDesktopWidget::setEditMode(const bool &mode)
{
    if (mode) {
        setState(DOCKED);
        prepareGeometryChange();
        this->setContentRect(d->mDockRect);
        this->setVisible(true);
    }

    d->mEditMode = mode;
}

bool AbstractDesktopWidget::editMode() const
{
    return d->mEditMode;
}

void AbstractDesktopWidget::setLabelName(const QString &name)
{
    d->mName = name;
}

QString AbstractDesktopWidget::label() const
{
    return d->mName;
}

void AbstractDesktopWidget::setContentRect(const QRectF &rect)
{
    d->mContentRect = rect;
    d->mBoundingRect = rect;

    prepareGeometryChange();
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    resetMatrix();
    update();
}

QRectF AbstractDesktopWidget::contentRect() const
{
    return d->mContentRect;
}

void AbstractDesktopWidget::setRect(const QRectF &rect)
{
    d->mBoundingRect = rect;

    prepareGeometryChange();
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    resetMatrix();
    update();
}

AbstractDesktopWidget::State AbstractDesktopWidget::state()
{
    return d->mWidgetState;
}

void AbstractDesktopWidget::setState(AbstractDesktopWidget::State s)
{
    d->mWidgetState = s;
    Q_EMIT stateChanged();
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

void AbstractDesktopWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void AbstractDesktopWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void AbstractDesktopWidget::setRotation(float angle)
{
    d->mAngle = angle;
    setCacheMode(ItemCoordinateCache);
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.rotate(d->mAngle, Qt::YAxis);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->mAngle >= 180) {
        if (state() == ROTATED) {
            setState(VIEW);
        } else {
            setState(ROTATED);
        }
        resetMatrix();
        //setCacheMode(DeviceCoordinateCache);
        d->mAngle = 0;
    }
}

float AbstractDesktopWidget::scaleFactorForWidth() const
{
    return contentRect().width() / boundingRect().width();
}

float AbstractDesktopWidget::scaleFactorForHeight() const
{
    return contentRect().height() / boundingRect().height();
}

void AbstractDesktopWidget::setChildWidetVisibility(bool show)
{
    Q_FOREACH(QGraphicsItem *item, this->childItems()) {
        (show) ? item->show() : item->hide();
    }
}

void AbstractDesktopWidget::setController(ControllerInterface *view_controller)
{
    d->mController = view_controller;
}

ControllerInterface *AbstractDesktopWidget::controller() const
{
    return d->mController;
}

} //namespace PlexyDesk
