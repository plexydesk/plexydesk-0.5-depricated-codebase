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

#ifndef PLEXY_DESKTOP_WIDGET_H
#define PLEXY_DESKTOP_WIDGET_H

#include <plexy.h>
#include <QtWidgets/QGraphicsObject>
#include <QtWidgets/QGraphicsView>

namespace PlexyDesk
{

class ControllerInterface;

class PLEXYDESKCORE_EXPORT AbstractDesktopWidget : public QGraphicsObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QRectF rect READ boundingRect() WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(float rotation READ rotation WRITE setRotation NOTIFY stateChanged)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public :
    enum State {
       DOCKED,
       VIEW,
       ROTATED
     };

    explicit AbstractDesktopWidget(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~AbstractDesktopWidget();

    virtual QRectF boundingRect() const;

    void setContentRect(const QRectF &rect);
    QRectF contentRect() const;
    void setRect(const QRectF &rect);

    void setDockRect(const QRectF &rect);
    QRectF dockRect() const;

    float rotation() const;
    void setRotation(float angle);

    float scaleFactorForWidth() const;
    float scaleFactorForHeight() const;

    void setChildWidetVisibility(bool show);

    void setController(ControllerInterface *view_controller);
    ControllerInterface * controller() const;

    void setLabelName(const QString &name);

    QString label() const;

    QString widgetID() const;

    State state();

    // TODO: move these out from here
    void setEditMode(const bool &mode);
    bool editMode() const;
    void setState(State s);

Q_SIGNALS:
    void closed(PlexyDesk::AbstractDesktopWidget *widget);
    void rectChanged();
    void stateChanged();
    void itemMoved(const QPointF &pos, const QString &widgetId);

protected:

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void paintRotatedView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintFrontView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintDockView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintEditMode(QPainter *painter, const QRectF &rect) = 0;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    class PrivateAbstractDesktopWidget;
    PrivateAbstractDesktopWidget *const d;
};
} //

#endif
