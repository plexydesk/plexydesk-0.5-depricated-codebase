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
#include <iconprovider.h>
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
      QString text;
      bool valid;
      IconProviderPtr iconprovider;
      uint id;
};

Icon::Icon(IconProviderPtr icon, const QRectF &rect, QWidget *embeddedWidget) : DesktopWidget(rect), d(new Private)
{
    d->valid = false;
    d->iconprovider = icon;
    d->id = 0;
     connect(d->iconprovider.data(),SIGNAL(iconPixmap(const QPixmap&, uint)),this, SLOT(onIconPixmap(const QPixmap&, uint)));
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
        setting.endGroup();
        d->valid = true;
    } else {
        QString name = d->mime.genericIconName();
    }
    d->text = iconname;
   // d->icon = QPixmap(applicationDirPath()+"/share/app-install/icons/"+iconname+".png");
    d->id = d->iconprovider->requestIcon(iconname, "32x32");
    qDebug()<<"Registereding -> Id "<<iconname<<d->id<<endl;

}

void Icon::onIconPixmap(const QPixmap& pixmap, uint id)
{

    if(!pixmap.isNull())  {
      d->icon = d->iconprovider->getIcon(d->id);
    }
    update();
}

bool Icon::isValid()
{
    return d->valid;
}

void Icon::paintBackSide(QPainter * painter,const QRectF& rect)
{
        if(!d->icon.isNull()) {
         int x = (this->boundingRect().width() - d->icon.width())/2;
         painter->drawPixmap(QRect(x, x , d->icon.width(), d->icon.height()) , d->iconprovider->getIcon(d->id));
        }
       // DesktopWidget::paintDockView(painter, rect);
}
void Icon::paintViewSide(QPainter * painter,const QRectF& rect)
{
       // DesktopWidget::paintDockView(painter, rect);
        if(!d->icon.isNull()) {
          int x = (this->boundingRect().width() - d->icon.width())/2;
          painter->drawPixmap(QRect(x, x , d->icon.width(), d->icon.height()) , d->iconprovider->getIcon(d->id));
        }
}
void Icon::paintDockView(QPainter * painter,const QRectF& rect)
{
    DesktopWidget::paintDockView(painter, rect);
   if(!d->icon.isNull()) {
         int x = (this->boundingRect().width() - d->icon.width())/2;
         painter->drawPixmap(QRect(x, x , d->icon.width(), d->icon.height()) , d->iconprovider->getIcon(d->id));
    }
}

}
