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

#include "imagepil.h"

#include <QtCore>
#include <QtGui>

ImagePile::ImagePile(QObject * object)
{
    flow = new PictureFlow(0);
    flow->setSlideSize(QSize(320/2, 240/2));
    flow->resize(350,150);
    flow->move(24,8);
//    emit dataChange();	
    widget =  new PlexyDesk::ImagePileWidget(QRectF(0, 0, 400, 200), flow);
}

ImagePile::~ImagePile()
{
delete flow;
}

void ImagePile::data(QVariant& data)
{
    QImage wall(QImage::fromData(data.toByteArray()));
    flow->addSlide(wall);     
    flow->showNext();
    qDebug()<<"Adding Slide"<<endl;;
}

QGraphicsItem * ImagePile::item()
{
    flickrEngine= loadData("flickerengine");
    if (flickrEngine) {
        connect(flickrEngine,SIGNAL(data(QVariant&)),this,SLOT(data(QVariant&)));
    }else {
        qDebug()<<"DataSource Was Null"<<"ImagePile::ImagePile(QObject * object)"<<endl;;
    }

    PictureFlow * flow = new PictureFlow(0);
    return widget;//new PlexyDesk::ImagePileWidget(QRectF(0, 0, 400, 200), flow);
}

Q_EXPORT_PLUGIN2(ImagePile, ImagePile)

#include "imagepil.moc"
