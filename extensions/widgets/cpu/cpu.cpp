#include "cpu.h"
#include "cpuwidget.h"
#include <QtCore>
#include<QtGui>


Cpu::Cpu(QObject * object)
{

}

Cpu::~Cpu()
{

}

QGraphicsItem * Cpu::item()
{
return new  PlexyDesk::CpuWidget(QRectF(0,0,210,210),0);
}

Q_EXPORT_PLUGIN2(Cpu,Cpu)
#include "cpu.moc"
