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
#include "backdropitem.h"
#include "customproxy.h"
#include "pictureflow.h"

namespace PlexyDesk
{
    BackdropItem::BackdropItem(const QRectF &rect):QGraphicsRectItem(rect)
    {
        PictureFlow* w = new PictureFlow; 
        w->setSlideSize(QSize(320/2, 240/2));
        w->resize(QDesktopWidget().screenGeometry().width() ,240);
        w->setBackgroundColor(Qt::white);
        bg = QPixmap( QString(PLEXYPREIX)+"/share/plexy/skins/default/default.png"); 
        CustomProxy *proxyWidget  = new   CustomProxy ( this);
        proxyWidget->setWidget(w);
        proxyWidget->setGeometry(boundingRect().adjusted(25, 25, -25, -25));
        proxyWidget->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
 
        PlexyDesk::DataInterface * data = (PlexyDesk::DataInterface*)
        PlexyDesk::PluginLoader::getInstance()->instance("flickerengine");
         if (data) {
             connect(data,SIGNAL(data(QVariant&)),this, SLOT(data(QVariant&)));
     }
    
        for (int i = 0; i < 10 ; i++) {
            w->addSlide(QPixmap("/usr/share/plexy/skins/default/default.png"));
        }
        w->setCenterIndex(w->slideCount()/2);
        w->show();
    }
    
    BackdropItem::~BackdropItem()
    {

    }

    void BackdropItem::paint(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget *)
    {
        //   p->setOpacity(0.5);
         //  p->drawPixmap(QRect(0,0,159,174) ,QPixmap("/usr/share/plexy/skins/default/buttons/ikon-4.png"));
    }

    QRectF BackdropItem::boundingRect() const
    {
        return QRectF(0,0,QDesktopWidget().screenGeometry().width() ,240); 
    }

    void BackdropItem::data(QVariant& data)
    {
        qDebug()<<data<<endl;
    }

} // namespace PlexyDesk

#include "backdropitem.moc"
