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

#include <plexy.h>
#include <desktopwidget.h>
#include <config.h>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOptionGraphicsItem>

namespace PlexyDesk {

FriendItem::FriendItem(QGraphicsItem * parent)
{
	mShade = 0;
	setParentItem(parent);
	mPanel = QImage(QString(PLEXPREFIX) + "/theme/skins/default/widget/friendswidget/panel.png");
	mIcon = QPixmap(QString(PLEXPREFIX) + "/theme/skins/default/widget/friendswidget/face.png");
}

FriendItem::~FriendItem()
{
}

QRectF FriendItem::boundingRect() const
{
	return QRectF(0, 0, 183, 53);
}

void FriendItem::paint(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
{
    DesktopWidget *dw = static_cast<DesktopWidget *>(parentItem());

    if (dw->state() == DesktopWidget::DOCK)
        return;

    QRectF r = e->exposedRect;

    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(QRectF(0, 0, 183, 53), Qt::transparent);

    p->drawImage(QRect(0, 0, mPanel.width(), mPanel.height()), mPanel);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);

    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    if (mShade == 0) {
        p->drawPixmap(mPanel.rect(), QPixmap().fromImage(mPanel));
        mShade = 1;
    }
    p->restore();

    p->save();
    p->drawPixmap (QRect(8, 10, mIcon.width(), mIcon.height()), QPixmap(mIcon));
    p->restore();

    p->setRenderHints(QPainter::SmoothPixmapTransform |
                      QPainter::Antialiasing |
                      QPainter::HighQualityAntialiasing);

    p->setPen(QColor(255, 255, 255));
    p->setFont(QFont("Bitstream Charter", 11));
    p->drawText(QRect(45, 20, 150, 20), Qt::AlignLeft, mName);
}

void FriendItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  emit clicked();
}

void FriendItem::setName(const QString& name)
{
	mName = name;
}

void FriendItem::setIcon(QPixmap m)
{
	mIcon = m;
}

QString FriendItem::name() const
{
	return mName;
}

}// namespace PlexyDesk

#include "frienditem.moc"
