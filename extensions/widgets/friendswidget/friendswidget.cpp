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

#include "friendswidget.h"

#include <plexy.h>
#include <QPixmap>
#include <QBitmap>
#include <QPaintEvent>

namespace PlexyDesk {

FriendsWidget::FriendsWidget (const QRectF &rect, QWidget *widget)
    : DesktopWidget(rect,widget)
{
    mShade = 0;
    setPath(QString(PLEXPREFIX) + "/theme/skins/default/widget/friendswidget");
    setDockImage(QPixmap(mPrefix + "icon.png"));
    drawFriendsWidget();
}

void FriendsWidget::setPath(const QString& path)
{
    mPrefix = path + "/";
}

void FriendsWidget::drawFriendsWidget()
{
    //Friends
    FriendItem *fitem = new FriendItem(this);
    fitem->setPos(18, 50);
    fitem->setName("Siraj");
    //fitem->setIcon

    connect(fitem, SIGNAL (clicked()), this, SLOT (spin()));

    mItems[fitem->name()] = fitem;

    fitem = new FriendItem(this);
    fitem->setPos(18, 100);
    fitem->setName("Lahiru");
    //fitem->setIcon

    connect(fitem, SIGNAL (clicked()), this, SLOT (spin()));

    mItems[fitem->name()] = fitem;

    fitem = new FriendItem(this);
    fitem->setPos(18, 150);
    fitem->setName("Bud");
    //fitem->setIcon

    connect(fitem, SIGNAL (clicked()), this, SLOT (spin()));

    mItems[fitem->name()] = fitem;

    fitem = new FriendItem(this);
    fitem->setPos(18, 200);
    fitem->setName("Mani");
    //fitem->setIcon

    connect(fitem, SIGNAL (clicked()), this, SLOT (spin()));

    mItems[fitem->name()] = fitem;

    //Actions
    FriendItem *action = new FriendItem(this);
    action->setPos(18, 50);
    action->setName("Chat");
    action->setIcon(QPixmap(mPrefix + "chat.png"));
    action->hide();

    connect(action, SIGNAL (clicked()), this, SLOT (spin()));

    mActions[action->name()] = action;

    action = new FriendItem(this);
    action->setPos(18, 100);
    action->setName("Webcam");
    action->setIcon(QPixmap(mPrefix + "webcam.png"));
    action->hide();

    connect(action, SIGNAL (clicked()), this, SLOT (spin()));

    mActions[action->name()] = action;

    action = new FriendItem(this);
    action->setPos(18, 150);
    action->setName("Blog");
    action->setIcon(QPixmap(mPrefix + "blog.png"));
    action->hide();

    connect(action, SIGNAL (clicked()), this, SLOT (spin()));

    mActions[action->name()] = action;

    action = new FriendItem(this);
    action->setPos(18, 200);
    action->setName("Twitter");
    action->setIcon(QPixmap(mPrefix + "twitter.png"));
    action->hide();

    connect(action, SIGNAL (clicked()), this, SLOT (spin()));

    mActions[action->name()] = action;

    m_main_bg = QImage (mPrefix + "default.png");
}

FriendsWidget::~FriendsWidget ()
{
}

void FriendsWidget::paintExtFace (QPainter *p,
                                  const QStyleOptionGraphicsItem *e,
                                  QWidget *)
{
    qDebug() << "drawing face" << __PRETTY_FUNCTION__ << __LINE__;
    QRectF r = e->exposedRect;

    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(rect(), Qt::transparent);

    p->drawImage (QRect(0, 0, m_main_bg.width(), m_main_bg.height()), m_main_bg);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);

    p->save ();
    p->setRenderHint (QPainter::SmoothPixmapTransform);
    if (mShade == 0) {
        p->drawPixmap (m_main_bg.rect(), QPixmap().fromImage(m_main_bg));
        mShade = 1;
    }

/*    p->setRenderHints(QPainter::SmoothPixmapTransform |
                      QPainter::Antialiasing |
                      QPainter::HighQualityAntialiasing);*/

    p->setPen(QColor(255, 255, 255));
    p->setFont(QFont("Bitstream Charter", 10, QFont::Bold));
    p->drawText(QRect(60, 5, 100, 64), Qt::AlignCenter, "Plexy-Friends");

    QMapIterator<QString, QGraphicsItem*> i(mItems);
    while (i.hasNext()) {
        i.next();
        (i.value())->show();
    }

    QMapIterator<QString, QGraphicsItem*> ix(mActions);
    while (ix.hasNext()) {
        ix.next();
        (ix.value())->hide();
    }
    p->restore ();
}

void FriendsWidget::paintExtDockFace(QPainter *p,
                                     const QStyleOptionGraphicsItem *e,
                                     QWidget *)
{
    qDebug() << "drawing dock" << __PRETTY_FUNCTION__ << __LINE__;
 	p->setRenderHints(QPainter::SmoothPixmapTransform |
                      QPainter::Antialiasing |
                      QPainter::HighQualityAntialiasing);

    p->save();
    QMapIterator<QString, QGraphicsItem*> i(mItems);
    while (i.hasNext()) {
        i.next();
        (i.value())->hide();
    }

    QMapIterator<QString, QGraphicsItem*> ix(mActions);
    while (ix.hasNext()) {
        ix.next();
        (ix.value())->hide();
    }
    p->restore();
}

void FriendsWidget::paintExtBackFace(QPainter *p,
                                     const QStyleOptionGraphicsItem *e,
                                     QWidget *)
{
    qDebug() << "drawing back face" << __PRETTY_FUNCTION__ << __LINE__;
    QRectF r = e->exposedRect;

    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(rect(), Qt::transparent);

    p->drawImage (QRect(0, 0, m_main_bg.width(), m_main_bg.height()), m_main_bg);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);
    p->save ();
    p->setRenderHint (QPainter::SmoothPixmapTransform);

    if (mShade == 0) {
        p->drawPixmap (m_main_bg.rect (), QPixmap ().fromImage(m_main_bg));
        mShade = 1;
    }

    p->restore ();

    p->setRenderHints(QPainter::SmoothPixmapTransform |
                      QPainter::Antialiasing |
                      QPainter::HighQualityAntialiasing);

    p->setPen(QColor(255, 255, 255));
    p->setFont(QFont("Bitstream Charter", 10, QFont::Bold));
    p->drawText(QRect(60, 5, 100, 64), Qt::AlignCenter, "Plexy-Friends" );

    QMapIterator<QString, QGraphicsItem*> i(mItems);
    while (i.hasNext()) {
        i.next();
        (i.value())->hide();
    }

    QMapIterator<QString, QGraphicsItem*> ix(mActions);
    while (ix.hasNext()) {
        ix.next();
        (ix.value())->show();
    }
}

} // namespace PlexyDesk

#include "friendswidget.moc"
