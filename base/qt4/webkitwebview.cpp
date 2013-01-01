#include "webkitwebview.h"
#include <QObject>
#include <QWebFrame>

namespace PlexyDesk {

class WebKitWebView::PrivateWebKitWebView
{
public:
    PrivateWebKitWebView() {}
    ~PrivateWebKitWebView() {}

    QGraphicsWebView *mView;
    QHash<QString, QObject*> mObjects;
};

WebKitWebView::WebKitWebView(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent),
    d(new PrivateWebKitWebView)
{
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    d->mView = new QGraphicsWebView (this);
    d->mView->resize (rect.size());
    d->mView->setResizesToContents(false);
    d->mView->setGeometry(rect);
    d->mView->show();
    d->mView->setPos(0.0, 24.0);

    if (d->mView->page() && d->mView->page()->mainFrame()) {
        connect(d->mView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(addJavaScriptObject()));
    }
}

WebKitWebView::~WebKitWebView()
{
    delete d;
}

void WebKitWebView::setUrl(const QUrl &url)
{
    d->mView->setUrl(url);
}

void WebKitWebView::injectQObject(const QString &objectName, QObject *object)
{
    d->mObjects[objectName] = object;
}

void WebKitWebView::addJavaScriptObject()
{
    qDebug() << Q_FUNC_INFO << "Initiate WebKit Bridge";

    Q_FOREACH(const QString &objectName, d->mObjects.keys()) {
        if (d->mObjects[objectName]) {
            d->mView->page()->mainFrame()->addToJavaScriptWindowObject(objectName, d->mObjects[objectName]);
        }
    }
}

}
