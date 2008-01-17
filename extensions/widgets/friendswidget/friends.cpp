#include "friends.h"
#include "friendswidget.h"
#include <QtCore>
#include<QtGui>


Friends::Friends(QObject * object):QObject(object)
{

}

Friends::~Friends()
{

}

QGraphicsItem * Friends::backdrop()
{
return new  PlexyDesk::FriendsWidget(QRectF(0,0,220,290),0);
}

Q_EXPORT_PLUGIN2(Friends,Friends)
#include "friends.moc"
