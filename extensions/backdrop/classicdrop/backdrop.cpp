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
#include "backdrop.h"
#include <desktopwidget.h>

ClassicBackdrop::ClassicBackdrop(QObject * object)
{
    bg = QPixmap("/usr/share/plexy/skins/default/default.png");
    width = QDesktopWidget().availableGeometry().width();
    height = QDesktopWidget().availableGeometry().height();
    img = QImage(width,height,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&img);
    p.drawPixmap(QRect(0,0,width,height),bg);
    paint.setTextureImage(img);
    flickrEngine= loadData("flickerengine");
    if (flickrEngine) {
        connect(flickrEngine,SIGNAL(data(QVariant&)),this,SLOT(data(QVariant&)));
    }else {
        qDebug("DataSource Was Null");
    }

}

ClassicBackdrop::~ClassicBackdrop()
{

}

void ClassicBackdrop::data(QVariant& data)
{
    QImage wall(QImage::fromData(data.toByteArray()));
    QPainter p ;
    p.begin(&img);
    p.drawImage(QRect(0,0,width,height),wall);
    p.end();
    qDebug()<<img.size()<<endl;
    paint.setTextureImage(img);
    emit dataChange();
}

void ClassicBackdrop::render(QPainter *p,QRectF r)
{
    p->fillRect(r.x(),r.y(),r.width(),r.height(),paint);
}

Q_EXPORT_PLUGIN2(ClassicBackdrop,ClassicBackdrop)
#include "backdrop.moc"
