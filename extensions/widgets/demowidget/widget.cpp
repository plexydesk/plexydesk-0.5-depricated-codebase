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
    /*
QWebView *view = new QWebView(0);
view->load(QUrl("http://www.google.com/m"));
view->resize(200,500);
//view->show();
*/
    QCalendarWidget *date = new QCalendarWidget(0);
    date->setWindowOpacity(0.2);
    date->resize(300,200);

return new  PlexyDesk::WidgetItem(QRectF(0,0,date->width(),date->height()),date);
}

Q_EXPORT_PLUGIN2(WidgetDemo,WidgetDemo)
#include "widget.moc"
