

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
#ifndef PLEXY_DESKTOP_VIEW_H
#define PLEXY_DESKTOP_VIEW_H

#include <QtCore>
#include <QtGui>


class WebView : public QGraphicsView
{
        Q_OBJECT
public:
        WebView(QGraphicsScene * scene = new QGraphicsScene(), QWidget * parent = 0);
        virtual ~WebView();

        void addWallpaper(const QString& path);
        void addRender(QGraphicsScene * scene);
        void addExtension(const QString& name);
        void addCoreExtension(const QString& name);
        void addDesktopItem(QGraphicsItem * item);
        void addWebPage(QGraphicsItem * item);
public slots:
        void backgroundChanged();
        void zoomView(int);
        void updateReflect();

protected:
        void drawBackground(QPainter * painter, const QRectF& rect);
        void mousePressEvent(QMouseEvent *event);

private:
        class Private;
        Private * const d;
        void setTopMostWidget(const QPoint &pt);
};

#endif
