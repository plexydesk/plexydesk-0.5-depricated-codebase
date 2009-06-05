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

#ifndef ICON_H
#define ICON_H

#include <QGraphicsRectItem>

#include <plexy.h>
#include <plexyconfig.h>
#include <desktopwidget.h>
namespace PlexyDesk
{
   class VISIBLE_SYM Icon : public DesktopWidget
   {
       Q_OBJECT
   public:
       Icon(const QRectF &rect, QWidget *embeddedWidget = 0);
       virtual ~Icon();
       virtual void setContent(const QString& path);

       virtual void paintBackSide(QPainter * painter,const QRectF& rect);
       virtual void paintViewSide(QPainter * painter,const QRectF& rect);
       virtual void paintDockView(QPainter * painter,const QRectF& rect);
   private:
        class Private;
        Private * const d;

    };
}
#endif // ICON_H
