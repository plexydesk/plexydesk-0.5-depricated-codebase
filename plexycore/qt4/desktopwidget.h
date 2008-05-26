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
#include <plexy.h>
#include <plexyconfig.h>
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
class DesktopWidget : public QObject, public QGraphicsRectItem
{
        Q_OBJECT
public:
        typedef enum {
                DOCK,
                NORMALSIDE,
                BACKSIDE
        } State;

        /** \brief Constructor
        * \param rect Defines the bounding rectangular area of a
        * desktop widget
        * \param embeddedWidget an optional parameter to embed a regular
        * QWidget.
        *
        * This will construct a extension with default settings with an
        * optional QWidget embedded
        */

        DesktopWidget(const QRectF &rect, QWidget *embeddedWidget = 0);

        virtual ~DesktopWidget();

        QString applicationDirPath();

        virtual void paintBackSide (QPainter * painter,const QRectF& rect);
        virtual void paintViewSide (QPainter * painter,const QRectF& rect);
        virtual void paintDockView (QPainter * painter,const QRectF& rect);

        void drawBackdrop(bool);
        void setState(State s);
        void configState(State s);
        State state();
        void setDockImage(QPixmap);
        void setFaceImage(QPixmap);
        void setBackFaceImage(QPixmap );

        virtual QRectF boundingRect() const;

public slots:
        void zoomIn(int);
        void zoomOut(int);
        void zoomDone();
        void spin();

protected:
        virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event );
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event);
        virtual void paintExtFace(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) {}
        virtual void paintExtBackFace(QPainter * painter,
                                      const QStyleOptionGraphicsItem * option,
                                      QWidget * widget = 0) {}
        virtual void paintExtDockFace(QPainter * painter,
                                      const QStyleOptionGraphicsItem * it,
                                      QWidget * widget = 0) {}
private:
        class Private ;
        Private * const d;
};
}//

#endif
