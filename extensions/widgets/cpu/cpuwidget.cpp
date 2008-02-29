/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include <plexy.h>
#include "cpuwidget.h"
#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <math.h>
#include <QBitmap>

namespace PlexyDesk
{

CpuWidget::CpuWidget (const QRectF &rect, QWidget *widget):
DesktopWidget(rect,widget)
{
  shade == 0;
  getBaseData();

  setPath("/usr/share/plexy/skins/default/cpu/");
  setDockImage(QPixmap(prefix + "icon.png"));
  drawCpuWidget();
}

void CpuWidget::setPath(QString str)
{
  prefix  = str+"/";
}

void 
CpuWidget::drawCpuWidget()
{
 _cpu_bg = QImage (prefix + "background.png");

/*gloss stuff*/
  gloss = QImage (prefix + "gloss.png");;

  thedot = QPixmap (prefix + "dot.png");

  _meter_hand = QPixmap ().fromImage(QImage (prefix + "needle.png"));
  meter_timer = new QTimer (this);
  connect (meter_timer, SIGNAL (timeout ()), this, SLOT (drawMeter()));
  meter_timer->start (1500);
}


CpuWidget::~CpuWidget ()
{

}


void
CpuWidget::drawMeter()
{
 update ();
 getNxtData();
}


void
CpuWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *)
{

  QRectF r  = e->exposedRect;
    p->setOpacity(0.8);
  p->setCompositionMode(QPainter::CompositionMode_Source);
  p->fillRect(rect(), Qt::transparent);

  p->drawImage (QRect(0,0,_cpu_bg.width(),_cpu_bg.height()),_cpu_bg);
  p->setCompositionMode(QPainter::CompositionMode_SourceOver);
  p->setBackgroundMode(Qt::TransparentMode);
  p->save ();
  p->setRenderHint (QPainter::SmoothPixmapTransform);
  if (shade == 0)
    {
      p->drawPixmap (_cpu_bg.rect (), QPixmap ().fromImage(_cpu_bg));
      shade = 1;
    }
  p->restore ();

/*Draw Needle*/

  p->save ();
  p->setRenderHint (QPainter::SmoothPixmapTransform);
  p->translate (_cpu_bg.width () / 2, _cpu_bg.width () / 2);
  p->rotate (230+(2.6*percen));//130 230
  p->drawPixmap (QRect
		(-(ceil (_meter_hand.width () / 2)),
		 -(_meter_hand.height () - 15), _meter_hand.width (),
		 _meter_hand.height ()), _meter_hand);
  p->restore ();

  p->save ();

  p->translate (_cpu_bg.width () / 2, _cpu_bg.width () / 2);
  p->drawPixmap (QRect
		(-(thedot.width () / 2), -(thedot.height () / 2),
		 thedot.width (), thedot.height ()), QPixmap (thedot));
  p->restore ();

  p->drawPixmap (QRect (10, 10, gloss.width (), gloss.height ()),
		QPixmap ().fromImage(gloss));

  p->save ();

  p->restore ();

  p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);

  p->setPen(QColor(16,90,147));
  p->setFont(QFont("Bitstream Charter",12,QFont::Bold));
  p->drawText(QRect(70,108,64,64), Qt::AlignCenter ,QString("%1").arg(percen)+"%" );

  p->setPen(QColor(255,255,255));
  p->setFont(QFont("Bitstream Charter",10,QFont::Bold));
  p->drawText(QRect(70,130,64,64), Qt::AlignCenter ,"CPU" );
}


void
CpuWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *)
{
 	/*p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
		
	p->setPen(QColor(255,255,255));
	p->setFont(QFont("Bitstream Charter",15));
	p->drawText(QRect(8,5,64,64), Qt::AlignCenter ,"Cpu\n"+QString("%1").arg(percen)+"%" );*/
}

void
CpuWidget::getBaseData()
{
	QFile file("/proc/stat");
     	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug()<<"Error opening /proc/stat !";
        	return;
	}

     	QTextStream in(&file);
        QString line = in.readLine();

	QStringList cpu_list = line.split(" ");

	user_a = cpu_list.at(2).toLong();
	nice_a = cpu_list.at(3).toLong();
	sys_a = cpu_list.at(4).toLong();
	idle_a = cpu_list.at(5).toLong();
}

void
CpuWidget::getNxtData()
{
	QFile file("/proc/stat");
     	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug()<<"Error opening /proc/stat !";
        	return;
	}

     	QTextStream in(&file);
        QString line = in.readLine();

	QStringList cpu_list = line.split(" ");

	user_b = cpu_list.at(2).toLong();
	nice_b = cpu_list.at(3).toLong();
	sys_b = cpu_list.at(4).toLong();
	idle_b = cpu_list.at(5).toLong();

	usage = (user_b-user_a) + (nice_b-nice_a) + (sys_b-sys_a);
	total = usage + (idle_b-idle_a);

	percen = (100*usage)/total;

	getBaseData();
}

} // namespace PlexyDesk
#include "cpuwidget.moc"
