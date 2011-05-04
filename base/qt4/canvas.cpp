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

#include <canvas.h>
#include <plexyconfig.h>
#include <desktopwidget.h>

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QDeclarativeEngine>
#include <QGraphicsObject>
#include <QDeclarativeComponent>
#include <QPainter>
#include <QPaintEngine>
#include <GL/gl.h>  // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
#include <QTimer>



namespace PlexyDesk
{
class Canvas::Private
{
public:
    Private() {
    }
    ~Private() {
        mQMLWidgets.clear();
    }
    QList<DesktopWidget *> mQMLWidgets;
};

Canvas::Canvas(QObject *parent) : QGraphicsScene(parent), d(new Private)
{
}

Canvas::~Canvas()
{
    delete d;
}

void Canvas::drawBackground(QPainter *painter, const QRectF &rect)
{
    qDebug() << Q_FUNC_INFO << rect;
    //painter->fillRect(rect, QColor(0.0, 255, 0.0));

    painter->beginNativePainting();

    glMatrixMode(GL_PROJECTION);
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    //glPushMatrix();
    glLoadIdentity();
   // gluPerspective(70, width() / height(), 0.01, 1000);
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glEnable(GL_MULTISAMPLE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(0.25, 0.25, 0.0);
        glVertex3f(0.75, 0.25, 0.0);
        glVertex3f(0.75, 0.75, 0.0);
        glVertex3f(0.25, 0.75, 0.0);
        glDisable(GL_MULTISAMPLE);
        glEnd();

        glFlush();


        painter->endNativePainting();
    QTimer::singleShot(20, this, SLOT(update()));
}

void Canvas::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event)
        event->accept();
}

void Canvas::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (!event)
        return;
    event->acceptProposedAction();
    const QUrl droppedFile = event->mimeData()->urls().at(0).toString(QUrl::StripTrailingSlash |
            QUrl::RemoveScheme);
    if (droppedFile.toString().contains(".qml")) {

        qDebug() << Q_FUNC_INFO << droppedFile;
        DesktopWidget *parent = new DesktopWidget(QRectF(0,0,0,0));
        parent->loadQML(droppedFile);
        addItem(parent);
        d->mQMLWidgets.append(parent);
        return;
    }
    if (event->mimeData()->hasUrls()) {
        Config::getInstance()->setWallpaper(event->mimeData()->urls().at(0).toString(QUrl::StripTrailingSlash | QUrl::RemoveScheme));
    }
}

} /* PlexyDesk */

