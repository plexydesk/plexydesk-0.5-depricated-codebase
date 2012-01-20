#include <qmldesktopwidget.h>

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
};


QmlDesktopWidget::QmlDesktopWidget(const QRectF &rect, QWidget *widget, QDeclarativeItem *parent) :
    DesktopWidget(rect, widget, parent),
    d(new PrivateQmlDesktopWidget)
{
    d->mQmlEngine = Config::getInstance()->newQmlEngine();
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
}

void QmlDesktopWidget::onQuit()
{
    qDebug() << Q_FUNC_INFO << endl;
    d->mQmlChild->hide();
    scene()->removeItem(d->mQmlChild);
    d->mQmlChild->setParentItem(0);
    Q_EMIT close();
}

void QmlDesktopWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{ 
    /* When the user clicks the widget is in NORMAL mode
     * So we hide the child widget and when he clicks 
     * We show the qml Child of the widget
     */
    if (state() == DesktopWidget::VIEW)
        d->mQmlChild->hide();
    else
        d->mQmlChild->show();
    
    DesktopWidget::mouseDoubleClickEvent(event);
}
    
}
