#include "clock.h"
#include "clockitem.h"
#include "clockwidget.h"
#include <QtCore>
#include<QtGui>


Clock::Clock(QObject * object)
{

}

Clock::~Clock()
{

}

QGraphicsItem * Clock::item()
{

return new  PlexyDesk::ClockWidget(QRectF(0,0,210,210),0);
}

Q_EXPORT_PLUGIN2(Clock,Clock)
#include "clock.moc"
