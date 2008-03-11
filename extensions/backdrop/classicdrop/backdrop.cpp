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
#include <config.h>
#include <desktopwidget.h>

ClassicBackdrop::ClassicBackdrop(QObject * object)
{
    bg = QPixmap( QString(PLEXPREFIX)+"/share/plexy/skins/default/default.png");
    width = QDesktopWidget().availableGeometry().width();
    height = QDesktopWidget().availableGeometry().height();
#ifdef Q_WS_MAC
    width  = QDesktopWidget().screenGeometry().width();
    height = QDesktopWidget().screenGeometry().height();
#endif
      img = QImage(width,height,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&img);
    p.drawPixmap(QRect(0,0,width,height),bg);
    paint.setTextureImage(img);
/*
    flickrEngine= loadData("flickerengine");
    if (flickrEngine) {
        connect(flickrEngine,SIGNAL(data(QVariant&)),
                this,SLOT(data(QVariant&)));
    }else {
    }

#ifdef Q_WS_WIN
    wallpaperWin = loadData("wallpaperchange-win32");
    if(wallpaperWin)
    {
        connect(wallpaperWin, SIGNAL(data(QVariant&)), this, SLOT(data(QVariant&)));
    }
    else
    {
        qDebug("Windows Wallpaper cannot load");
    }
#endif
*/
}

ClassicBackdrop::~ClassicBackdrop()
{

}

void ClassicBackdrop::data(QVariant& data)
{
    QImage wall = data.value<QImage>();
    if(wall.isNull())
    {
        wall = QImage::fromData(data.toByteArray());
    }
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

#ifdef Q_WS_MAC
   	p->fillRect(r.x(),r.y(),r.width(),r.height(),paint);		
#endif

#ifdef Q_WS_X11
   p->fillRect(r.x(),r.y(),r.width(),r.height(),paint);

#endif

#ifdef Q_WS_WIN
   p->fillRect(r.x(),r.y(),r.width(),r.height(),paint);
#endif
}

Q_EXPORT_PLUGIN2(ClassicBackdrop,ClassicBackdrop)
#include "backdrop.moc"
