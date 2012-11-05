#include "contactlistitem.h"
#include <nativestyle.h>
#include <QStyleOptionGraphicsItem>

class ContactListItem::PrivateContactListItem {
public:
    PrivateContactListItem() {}
    ~PrivateContactListItem() {}

    PlexyDesk::Style *mStyle;
    QString mName;

};

ContactListItem::ContactListItem(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d(new PrivateContactListItem)
{
    d->mStyle = new PlexyDesk::NativeStyle(this);
}

ContactListItem::~ContactListItem()
{
    delete d;
}

QRectF ContactListItem::boundingRect() const
{
    return QRectF(0.0, 0.0, 500, 50);
}

void ContactListItem::setName(const QString &name)
{
    d->mName = name;
}

void ContactListItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    PlexyDesk::StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = PlexyDesk::StyleFeatures::SF_FrontView;
    //d->mStyle->paintControlElement(PlexyDesk::Style::CE_Frame, feature, painter);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPen pen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    QFont font = QFont("", 20);
    painter->setFont(font);
    painter->setPen(pen);
    painter->drawText(option->exposedRect, Qt::AlignCenter| Qt::AlignVCenter, d->mName);

    feature.exposeRect = QRectF(4.0,0.0, option->exposedRect.width(), 1.0);
    d->mStyle->paintControlElement(PlexyDesk::Style::CE_Seperator, feature, painter);
}

void ContactListItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void ContactListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}
