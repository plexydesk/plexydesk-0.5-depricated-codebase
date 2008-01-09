#include "widget.h"
#include "widgetitem.h"
#include <QtWebKit>


WidgetDemo::WidgetDemo(QObject * object):QObject(object)
{

}

WidgetDemo::~WidgetDemo()
{

}

QGraphicsItem * WidgetDemo::backdrop()
{
QWebView *view = new QWebView(0);
view->load(QUrl("http://www.google.com/m"));
view->resize(200,500);
//view->show();
return new  PlexyDesk::WidgetItem(QRectF(0,0,200,500),view->page()->view());
}

Q_EXPORT_PLUGIN2(WidgetDemo,WidgetDemo)
#include "widget.moc"
