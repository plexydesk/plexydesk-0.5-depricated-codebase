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
#include <qmldesktopwidget.h>
#include <QDeclarativeComponent>
#include <plexyqmlglue.h>

namespace PlexyDesk
{
class QmlDesktopWidget::PrivateQmlDesktopWidget
{
public:
    PrivateQmlDesktopWidget() {}
    ~PrivateQmlDesktopWidget() {}
    QUrl mQmlUrl;
    QGraphicsObject *mQmlChild;
    QDeclarativeEngine *mQmlEngine;
    bool mHasBackground;
};


QmlDesktopWidget::QmlDesktopWidget(const QRectF &rect, QWidget *widget, QGraphicsObject *parent) :
    DesktopWidget(rect, parent),
    d(new PrivateQmlDesktopWidget)
{
    d->mQmlEngine = PlexyQmlGlue::qmlEngine();
    d->mQmlChild = 0;
}

QmlDesktopWidget::~QmlDesktopWidget()
{
    delete d;
}
    
QDeclarativeEngine *QmlDesktopWidget::engine() const
{
    return d->mQmlEngine;
}

void QmlDesktopWidget::setSourceUrl(const QUrl &url)
{
    d->mQmlUrl = url;
    qDebug() << Q_FUNC_INFO << url;
    
    if (d->mQmlChild) {
        delete d->mQmlChild;
    }
    
    QDeclarativeComponent component(d->mQmlEngine, QDir::cleanPath(
                                        url.toString(QUrl::StripTrailingSlash |
                                                     QUrl::RemoveScheme)));
    
    if (!component.isReady()) {
        if (component.isError()) {
            Q_FOREACH(QDeclarativeError error, component.errors()) {
                qDebug() << Q_FUNC_INFO << "Component Error: " << error.toString();
            }
        }
        return;
    }
    
    d->mQmlChild =
        qobject_cast<QGraphicsObject *>(component.create(d->mQmlEngine->rootContext()));
    QRectF objectRect = d->mQmlChild->boundingRect();
    
    setRect(objectRect);
    
    d->mQmlChild->setParentItem(this);
    d->mQmlChild->setFlag(QGraphicsItem::ItemIsFocusable, true);
    d->mQmlChild->setFlag(QGraphicsItem::ItemIsSelectable, true);
    d->mQmlChild->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    
    // forward signals
    connect(d->mQmlChild, SIGNAL(quit()), this, SLOT(onQmlQuit()));
    connect(this, SIGNAL(stateChanged()), this, SLOT(onStateChanged()));
}

bool QmlDesktopWidget::hasBackground() const
{
    return d->mHasBackground;
}

void QmlDesktopWidget::setHasBackground(bool background)
{
    enableDefaultBackground(background);
    d->mHasBackground = background;
}

void QmlDesktopWidget::onQuit()
{
    d->mQmlChild->hide();
    scene()->removeItem(d->mQmlChild);
    d->mQmlChild->setParentItem(0);
    Q_EMIT closed();
}

void QmlDesktopWidget::onStateChanged()
{
    if(d->mQmlChild) {
        if (state() == ROTATED) {
            d->mQmlChild->hide();
            enableDefaultBackground(true);
        } else if (state() == VIEW) {
            d->mQmlChild->show();
            if (!hasBackground())
                enableDefaultBackground(false);
        }
    }
}

void QmlDesktopWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{ 
    /* When the user clicks the widget is in NORMAL mode
     * So we hide the child widget and when he clicks 
     * We show the qml Child of the widget
     */

    AbstractDesktopWidget::mouseDoubleClickEvent(event);
}
    
}
