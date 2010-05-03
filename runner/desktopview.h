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

#include <QGraphicsView>

#include <plexy.h>

//TODO:
//move out of base library

namespace PlexyDesk
{
    class VISIBLE_SYM DesktopView : public QGraphicsView
    {
        Q_OBJECT
    public:
        DesktopView(QGraphicsScene * scene = new QGraphicsScene(), QWidget * parent = 0);
        virtual ~DesktopView();

        void addWallpaper(const QString& path);
        void addRender(QGraphicsScene * scene);
        void addExtension(const QString& name);
        void addCoreExtension(const QString& name);
        void addDesktopItem(QGraphicsItem * item);
        void enableOpenGL(bool);

    public slots:
        void backgroundChanged();
        void onNewWidget();
        void showIcon(int);
        void loadIcons();
        void iconLoaded();

    protected:
        void drawBackground(QPainter * painter, const QRectF& rect);
        void mousePressEvent(QMouseEvent *event);

    private:
        class Private;
        Private * const d;
        void setTopMostWidget(const QPoint &pt);
    };
} // namespace #ifndef

#endif
