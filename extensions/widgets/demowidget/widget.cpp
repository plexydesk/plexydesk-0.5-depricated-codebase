#include "widget.h"
#include "widgetitem.h"
//#include <QtWebKit>


WidgetDemo::WidgetDemo(QObject * object)
{

}

WidgetDemo::~WidgetDemo()
{

}

QGraphicsItem * WidgetDemo::item()
{
    QCalendarWidget *date = new QCalendarWidget(0);
    date->setWindowOpacity(0.2);
    date->resize(300,200);
    date->move(20,20);

return new  PlexyDesk::WidgetItem(QRectF(0,0,340,240),date);
}

Q_EXPORT_PLUGIN2(WidgetDemo,WidgetDemo)
#include "widget.moc"
