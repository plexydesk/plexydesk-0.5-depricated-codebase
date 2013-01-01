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

    virtual void setUrl(const QUrl &url);
    virtual void injectQObject(const QString &objectName, QObject *object);


public Q_SLOTS:
    void addJavaScriptObject();

private:
    class PrivateWebKitWebView;
    PrivateWebKitWebView *const d;

};
}

#endif // WEBKITWEBVIEW_H
