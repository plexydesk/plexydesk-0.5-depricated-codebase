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

void Canvas::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event)
        event->accept();
}

void Canvas::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event) {
        event->accept();
    }
    dragLeaveEvent(event);
}
void Canvas::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (!event) {
        return;
    } else {
        event->accept();
    }

    event->acceptProposedAction();
    if ((event->mimeData()->urls().count() <= 0 ) ||
            !event->mimeData()->hasUrls()) {
        return;
    }

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

