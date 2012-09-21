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
#include <QtGui>

class QGraphicsObject;

namespace PlexyDesk
{
class PLEXYDESK_EXPORT AbstractDesktopWidget : public QGraphicsObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QRectF rect READ boundingRect() WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(float rotation READ rotation WRITE setRotation NOTIFY stateChanged)

public :
    enum State {
       DOCKED,
       VIEW,
       ROTATED
     };

    AbstractDesktopWidget(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~AbstractDesktopWidget();

    virtual QRectF boundingRect() const;

    void setContentRect(const QRectF &rect);
    QRectF contentRect() const;
    void setRect(const QRectF &rect);

    void setDockRect(const QRectF &rect);
    QRectF dockRect() const;

    float rotation() const;
    void setRotation(float angle);

    void setChildWidetVisibility(bool show);

     /**
     * \brief Sets a label for the widget
     * \param name Name of the icon
     *
     * Label to display when the widget is in dock mode
     */
    void setLabelName(const QString &name);

     /**
     * \brief Returns the label text
     * \retuns the label name
     */
    QString label() const;

     /**
     *\brief Widget state
     * returns Retuns the state of the widget
     */
    State state();

    /**
     * \breif Changes the widget mode
     * \param mode True to put the widget in edit mode
     * and False if you want to change back to normal mode
     *
     * When you put the widget in edit mode it will go into
     * dock mode but if you click on the widget it will emit
     * the close signal
     */
    // TODO: move these out from here
    void setEditMode(const bool &mode);
    bool editMode() const;

Q_SIGNALS:
    void closed();
    void rectChanged();
    void stateChanged();

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void paintRotatedView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintFrontView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintDockView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintEditMode(QPainter *painter, const QRectF &rect) = 0;

    void setState(State s);
private:
    class PrivateAbstractDesktopWidget;
    PrivateAbstractDesktopWidget *const d;
};
} //

#endif
