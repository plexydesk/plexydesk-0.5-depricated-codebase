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

  p->restore ();

  p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);

  p->setPen(QColor(255,255,255));
  p->setFont(QFont("Bitstream Charter",10,QFont::Bold));
  p->drawText(QRect(50,0,64,64), Qt::AlignCenter ,"Lahiru" );
}

}// namespace PlexyDesk
#include "frienditem.moc"
