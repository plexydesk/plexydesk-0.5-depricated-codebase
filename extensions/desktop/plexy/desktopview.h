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
#include <desktopwidget.h>
#include <viewcontrollerplugin.h>
#include <abstractdesktopview.h>

using namespace PlexyDesk;

class DesktopView : public AbstractDesktopView
{
    Q_OBJECT
public:
    DesktopView(QGraphicsScene *scene = new QGraphicsScene(), QWidget *parent = 0);
    virtual ~DesktopView();

    enum {none, folder, qml, img};

    void addRender(QGraphicsScene *scene);
    void addExtension(const QString &name,
    const QString &layer = QLatin1String("Widgets"),
    const QPoint &pos = QPoint(0, 0),
    PlexyDesk::AbstractDesktopWidget::State state = PlexyDesk::AbstractDesktopWidget::DOCKED);
    void showLayer(const QString &name);
    void setThemePack(const QString &name);
    void addWallpaperItem();
    void registerPhotoDialog();
    PlexyDesk::ViewControllerPlugin *registerHandler(const QString &name, bool effects_on = false);

    void enableOpenGL(bool state);

#ifdef Q_WS_X11
    bool checkXCompositeExt();
    void redirectWindows();
    void loadWindows();
#endif

signals:
    void closeApplication();

public Q_SLOTS:
    void backgroundChanged();
    void onNewWidget();
    void closeDesktopWidget();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    //void paintEvent(QPaintEvent *event);
    int checkDropped (const QString &file);
    virtual void dropEvent(QDropEvent *event);
    virtual void dragEnterEvent (QDragEnterEvent * event);
    virtual void dragMoveEvent (QDragMoveEvent * event);


private:
    class Private;
    Private *const d;
    void setTopMostWidget(const QPoint &pt);
};

#endif
