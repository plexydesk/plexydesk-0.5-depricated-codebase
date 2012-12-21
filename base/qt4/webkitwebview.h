#ifndef WEBKITWEBVIEW_H
#define WEBKITWEBVIEW_H

#include <plexy.h>
#include <desktopwidget.h>
#include <QGraphicsWebView>

namespace PlexyDesk {
class WebKitWebView : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    WebKitWebView(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~WebKitWebView();



private:
    class PrivateWebKitWebView;
    PrivateWebKitWebView *const d;

};
}

#endif // WEBKITWEBVIEW_H
