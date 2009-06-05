/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "icon.h"
#include <qplexymime.h>
#include <QPainter>

namespace PlexyDesk
{

class Icon::Private
{
    public:
      Private(){}
      ~Private(){}
      QPixmap icon;
      QPlexyMime mime;
};

Icon::Icon(const QRectF &rect, QWidget *embeddedWidget) : DesktopWidget(rect), d(new Private)
{
}

Icon::~Icon()
{
    qDebug()<<"Delete Icon"<<endl;
}

void Icon::setContent(const QString& path)
{
    qDebug()<<d->mime.fromFileName(path)<<endl;
     QString iconname;
    if(d->mime.mimeType() == "application/x-desktop")
    {
        QSettings setting(path,  QSettings::IniFormat);
        setting.beginGroup("Desktop Entry");
        iconname = setting.value("Icon","").toString();
        qDebug()<<iconname<<endl;
        setting.endGroup();
    }
    d->icon = QPixmap(applicationDirPath()+"/share/app-install/icons/"+iconname+".png");
    qDebug()<<d->icon.isNull()<<endl;

}

void Icon::paintBackSide(QPainter * painter,const QRectF& rect)
{
        if(!d->icon.isNull()) {
            painter->drawPixmap(QRect(rect.x(), rect.y() , 32, 32) , d->icon);
        }
        DesktopWidget::paintDockView(painter, rect);
}
void Icon::paintViewSide(QPainter * painter,const QRectF& rect)
{
        DesktopWidget::paintDockView(painter, rect);
        if(!d->icon.isNull()) {
            painter->drawPixmap(QRect(rect.x(), rect.y() , 32, 32) , d->icon);
        }
}
void Icon::paintDockView(QPainter * painter,const QRectF& rect)
{
    DesktopWidget::paintDockView(painter, rect);
   if(!d->icon.isNull()) {
            painter->drawPixmap(QRect(rect.x(), rect.y() , 32, 32) , d->icon);
    }
}

}
