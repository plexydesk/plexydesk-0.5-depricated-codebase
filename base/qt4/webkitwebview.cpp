#include "webkitwebview.h"

namespace PlexyDesk {

class WebKitWebView::PrivateWebKitWebView
{
public:
    PrivateWebKitWebView() {}
    ~PrivateWebKitWebView() {}

    QGraphicsWebView *mView;
};

WebKitWebView::WebKitWebView(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent),
    d(new PrivateWebKitWebView)
{
    d->mView = new QGraphicsWebView (this);
    d->mView->resize (rect.size());
    d->mView->setResizesToContents(false);
    d->mView->setGeometry(rect);
    d->mView->show();
}

WebKitWebView::~WebKitWebView()
{
    delete d;
}

}
