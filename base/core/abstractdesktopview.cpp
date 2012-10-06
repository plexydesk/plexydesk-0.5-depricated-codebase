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

#include <config.h>

#include <QDir>
#include <QtDebug>
#include <QGLWidget>
#include <QFutureWatcher>
#include <QSharedPointer>
#include <QPropertyAnimation>
#include <QGraphicsGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QDomDocument>

#include <abstractdesktopwidget.h>
#include <controllerinterface.h>
#include <pluginloader.h>

#include "controllerinterface.h"
#include "abstractdesktopview.h"

/**
  \class PlexyDesk::AbstractDesktopView

  \brief Base class for creating DesktopViews

  \fn PlexyDesk::AbstractDesktopView::enableOpenGL()

  \param state Method to enable OpenGL rendering on the view, internally
               it assigns a QGLWidget to the viewport.

  \fn PlexyDesk::AbstractDesktopView::showLayer()

   \fn  PlexyDesk::AbstractDesktopView::addExtension()
   \brief Adds an Widget Extension to Plexy Desktop, give the widget
   name in string i.e "clock" or "radio", the internals will
   take care of the loading the widget if a plugin matching the name is
   found.

   \param name String name of the widget as specified by the desktop file

   \param layerName Name of the layer you want add the widget to

**/
namespace PlexyDesk
{

class AbstractDesktopView::PrivateAbstractDesktopView
{
public:
    PrivateAbstractDesktopView() {}
    ~PrivateAbstractDesktopView() {}

    QMap<QString, ControllerInterface*> mControllerMap;
    ControllerInterface *mDefaultViewController;
    AbstractDesktopWidget *mBackgroundItem;
    QDomDocument *mSessionTree;
    QDomElement mRootElement;
};

AbstractDesktopView::AbstractDesktopView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent), d(new PrivateAbstractDesktopView)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    d->mDefaultViewController = 0;
    d->mBackgroundItem = 0;

    d->mSessionTree = new QDomDocument("Session");
    d->mSessionTree->appendChild(d->mSessionTree->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    d->mRootElement = d->mSessionTree->createElement("session");
    d->mSessionTree->appendChild(d->mRootElement);
}

AbstractDesktopView::~AbstractDesktopView()
{
    delete d;
}

void AbstractDesktopView::enableOpenGL(bool state)
{
    if (state) {
        setViewport(new QGLWidget(QGLFormat(
                        QGL::DoubleBuffer )));
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    } else {
        setViewport(new QWidget);
        setCacheMode(QGraphicsView::CacheNone);
        setOptimizationFlags(QGraphicsView::DontSavePainterState);
        setOptimizationFlag(QGraphicsView::DontClipPainter);
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    }
}

bool AbstractDesktopView::setBackgroundController(const QString &controller_name)
{
    //TODO: error handling
    // delete the current background source before setting a new one

    if (d->mDefaultViewController)
        return 0;

    d->mDefaultViewController =
            qobject_cast<PlexyDesk::ControllerInterface*> (PlexyDesk::PluginLoader::getInstance()->instance(controller_name));

    if (!d->mDefaultViewController) {
        qWarning() << Q_FUNC_INFO << "Error Loading " << controller_name << " Controller";
        return 0;
    }

    d->mBackgroundItem = (AbstractDesktopWidget*) d->mDefaultViewController->defaultView();
    d->mBackgroundItem->setContentRect(this->sceneRect());
    d->mBackgroundItem->setRect(this->sceneRect());

    scene()->addItem(d->mBackgroundItem);

    d->mBackgroundItem->show();
    d->mBackgroundItem->setZValue(-1);

    d->mDefaultViewController->setViewport(this);
    return true;
}

void AbstractDesktopView::addController(const QString &controllerName)
{
    if (d->mControllerMap.keys().contains(controllerName))
        return;

    ControllerInterface *controller =
            qobject_cast<PlexyDesk::ControllerInterface*> (PlexyDesk::PluginLoader::getInstance()->instance(controllerName));

    if (!controller) {
        qWarning() << Q_FUNC_INFO << "Error loading extension";
        return;
    }

    connect(controller, SIGNAL(spawnView(AbstractDesktopWidget*)), this, SLOT(addWidgetToView(AbstractDesktopWidget*)));

    d->mControllerMap[controllerName] = controller;
    QGraphicsItem *defaultView = controller->defaultView();

    scene()->addItem(defaultView);

    defaultView->show();

    controller->setViewport(this);
    controller->setControllerName(controllerName);

    QDomElement widget = d->mSessionTree->createElement("widget");
    widget.setAttribute("controller", controllerName);
    QDomElement geometry = d->mSessionTree->createElement("geometry");
    geometry.setAttribute("x", defaultView->boundingRect().x());
    geometry.setAttribute("y", defaultView->boundingRect().y());
    widget.appendChild(geometry);
    d->mRootElement.appendChild(widget);

    qDebug() << Q_FUNC_INFO << d->mSessionTree->toString();
}

void AbstractDesktopView::dropEvent(QDropEvent *event)
{
    if (this->scene()) {
        QList<QGraphicsItem *> items = scene()->items(event->pos());

        Q_FOREACH(QGraphicsItem *item, items) {

            QGraphicsObject *itemObject = item->toGraphicsObject();

            if (!itemObject)
                continue;

            AbstractDesktopWidget *widget = qobject_cast<AbstractDesktopWidget*> (itemObject);

            if (!widget || !widget->controller() || (widget->controller() == d->mDefaultViewController))
                continue;

            widget->controller()->handleDropEvent(widget, event);
            return;
        }
    }

    if (d->mDefaultViewController) {
        d->mDefaultViewController->handleDropEvent(d->mBackgroundItem, event);
    }

    event->acceptProposedAction();
}

void AbstractDesktopView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void AbstractDesktopView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void AbstractDesktopView::addWidgetToView(AbstractDesktopWidget *widget)
{
    QGraphicsItem *item = (AbstractDesktopWidget*) widget;
    scene()->addItem(item);
    item->show();
}

void AbstractDesktopView::sessionDataForController(const QString &controllerName, const QString &key, const QString &value)
{
    qDebug() << Q_FUNC_INFO << "::" << controllerName<<":" <<key << ":" << value;
}

}
