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

/**
 * \class DesktopWidget
 *
 * \ingroup PlexyDesk
 *
 * \brief Base class for Visual Desktop Extensions
 * DesktopWidget defines all the common features of a visual desktop extension.
 * When writing visual extensions, the user must inherit from this class.
 *
 */

#ifndef PLEXY_DESKTOP_WIDGET_H
#define PLEXY_DESKTOP_WIDGET_H

#include <QGraphicsRectItem>
#include <plexy.h>
#include <plexyconfig.h>
#include "shaders/shadereffectitem.h"


namespace PlexyDesk
{
class PLEXYDESK_EXPORT AbstractDesktopWidget : public ShaderEffectItem
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)

public :
    enum State {
       DOCKED,
       VIEW,
       ROTATED
     };

    /**
     * \brief Base Desktop widget
     * \param rect Defines the bounding rectangular area of a
     * desktop widget
     * \param embeddedWidget an optional parameter to embed a regular
     * QWidget.
     *
     * This will construct a extension with default settings with an
     * optional QWidget embedded
     */

    AbstractDesktopWidget(const QRectF &rect, QWidget *embeddedWidget = 0, QDeclarativeItem *parent = 0);

    virtual ~AbstractDesktopWidget();

    /**
     * \breif Bounding Rectangle of the Widget
     * @returns The bounding Rectangle of the widget.
     */

    virtual QRectF boundingRect() const;
    QRectF rect() const;

     /**
     *\brief Widgets Content rectangle
     * 
     * Set the content rectangle of the widget
     * in situations where the widget can't 
     * find out by it self
     */
    void setContentRect (const QRectF &rect);

     /**
     * \brief Sets the Widget rectangle defined by rect
     * \param rect Rectangle to use
     */
    void setRect(const QRectF &rect);

     /**
     * \brief Sets a icon for the widget
     * \param name Name of the icon
     * 
     * Icon to display when the widget is in dock mode
     */
    void setIconName(const QString &name);

     /**
     * \brief Name of the Widget Icon
     * \retuns Icon name 
     */
    QString iconName() const;

     /** 
     *\brief Widget state
     * returns Retuns the state of the widget
     */
    State state();
    void configState(State s);

    void setDockBackground(QPixmap);
    void setWidgetBackground(QPixmap);
    void setBacksideBackground(QPixmap);

    /**
     * \breif Changes the widget mode
     * \param mode True to put the widget in edit mode
     * and False if you want to change back to normal mode
     * 
     * When you put the widget in edit mode it will go into 
     * dock mode but if you click on the widget it will emit
     * the close signal
     */

    void setEditMode(const bool &mode);
    bool editMode() const;

public Q_SLOTS:
    void zoomIn(int);
    void zoomOut(int);
    void zoomDone();
    void spin();
    void pressHoldTimeOut();

Q_SIGNALS:
    void closed();
    void rectChanged();

private:
    void setState(State s);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void paintRotatedView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintFrontView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintDockView(QPainter *painter, const QRectF &rect) = 0;
    virtual void paintEditMode(QPainter *painter, const QRectF &rect) = 0;

private:
    class Private;
    Private *const d;
};
} //

#endif
