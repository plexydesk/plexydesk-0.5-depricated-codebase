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
#include "desktopview.h"
#include <backdropfactory.h>
#include <abstractplugininterface.h>
#include <datainterface.h>
#include <desktopwidget.h>
#include <QGLWidget>


namespace PlexyDesk
{
class  DesktopView::Private
{
    public:
    Private(){}
    ~Private(){}
    ExtensionProducer<BackdropInterface> fact;
    BackdropInterface * bIface ;
    WidgetInterface * widgets;       
    DataInterface * data;       
    BackdropFactory * bgfact;
};

DesktopView::DesktopView ( QGraphicsScene * scene, QWidget * parent ):QGraphicsView(scene,parent),d(new Private)
{
       setWindowFlags(Qt::FramelessWindowHint);
       setAttribute(Qt::WA_ContentsPropagated );
       //scene->setBackgroundBrush(QColor(Qt::transparent));
       setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
       setFrameStyle(QFrame::NoFrame);
//       setViewport(new QGLWidget);
       setAlignment(Qt::AlignLeft | Qt::AlignTop);
       d->bIface  = 0;
       d->widgets = 0;
       d->data = 0;
       d->bIface = (BackdropInterface*)PluginLoader::getInstance()->instance("classicbackdrop");
       if (d->bIface) {
          connect(d->bIface,SIGNAL(dataChange()),this,SLOT(backgroundChanged()));
       }
}

DesktopView::~DesktopView()
{
    delete d;
}

void DesktopView::backgroundChanged()
{
    invalidateScene();
    scene()->update();
}

void DesktopView::addExtension(const QString& name)
{
        d->widgets = (WidgetInterface*) PluginLoader::getInstance()->instance(name);
        if (d->widgets){
                //scene()->addItem(d->widgets->item());
		DesktopWidget * widget = (DesktopWidget*) d->widgets->item();		
		if(widget){
		      widget->configState(DesktopWidget::DOCK);
		      scene()->addItem(widget);
		}
	}
}


void DesktopView::drawBackground ( QPainter * painter, const QRectF & rect )
{
    painter->save();
    painter->setClipRect(rect);
    if(d->bIface){
  	d->bIface->render(painter,QRectF(rect.x(),rect.y(),rect.width(),rect.height()));
    }

    painter->restore();
}


}
#include "desktopview.moc"
