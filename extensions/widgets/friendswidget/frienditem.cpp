/***************************************************************************
 *   Copyright (C) 2008 by Lahiru Lakmal Priyadarshana                     *
 *   llahiru@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "frienditem.h"
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOptionGraphicsItem>
#include <plexy.h>

namespace PlexyDesk
{

FriendItem::FriendItem(QGraphicsItem * parent)
{
	shade=0;
	setParentItem (parent);
	panel = QImage ("/usr/share/plexy/skins/default/friendswidget/panel.png");
	icon = QPixmap ("/usr/share/plexy/skins/default/friendswidget/face.png");
}

FriendItem::~FriendItem()
{

}

QRectF FriendItem::boundingRect() const
{
	return QRectF(0,0,183,53);
}

void FriendItem::paint(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
{
  QRectF r  = e->exposedRect;

  p->setCompositionMode(QPainter::CompositionMode_Source);
  p->fillRect(QRectF(0,0,183,53), Qt::transparent);

  p->drawImage (QRect(0,0,panel.width(),panel.height()),panel);
  p->setCompositionMode(QPainter::CompositionMode_SourceOver);
  p->setBackgroundMode(Qt::TransparentMode);
  p->save ();
  p->setRenderHint (QPainter::SmoothPixmapTransform);
  if (shade == 0)
    {
      p->drawPixmap (panel.rect (), QPixmap ().fromImage(panel));
      shade = 1;
    }
  p->restore ();

  p->save ();

  p->drawPixmap (QRect(8,10,icon.width (), icon.height ()), QPixmap (icon));

  p->restore ();

  p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);

  p->setPen(QColor(255,255,255));
  p->setFont(QFont("Bitstream Charter",11));
  p->drawText(QRect(45,20,150,20), Qt::AlignLeft ,name );
}

void FriendItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  emit clicked();
}

void FriendItem::setName(QString n)
{
	name=n;
}

void FriendItem::setIcon(QPixmap m)
{
	icon=m;
}

QString FriendItem::getName()
{
	return name;
}

}// namespace PlexyDesk
#include "frienditem.moc"
