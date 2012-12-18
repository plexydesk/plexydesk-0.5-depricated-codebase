#include "contactlistitem.h"
#include <nativestyle.h>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

#include <button.h>

class ContactListItem::PrivateContactListItem {
public:
    PrivateContactListItem() {}
    ~PrivateContactListItem() {}

    QPixmap genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap);

    PlexyDesk::Style *mStyle;
    QString mName;
    QString mStatusMessage;
    QPixmap mPixmap;
    QString mID;
    QPixmap mAvatar;

    PlexyDesk::Button *mButton;
};

ContactListItem::ContactListItem(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d(new PrivateContactListItem)
{
    d->mStyle = new PlexyDesk::NativeStyle(this);
    d->mButton = new PlexyDesk::Button(this);
    d->mButton->setLabel(tr("Add"));
    d->mButton->setSize (QSize (65, 20));
    d->mButton->setPos((boundingRect().width() - d->mButton->boundingRect().width()) - 10,
                       (boundingRect().height() / 2) - (d->mButton->boundingRect().height() / 2) );

    d->mButton->show();

    d->mButton->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    connect (d->mButton, SIGNAL(clicked()), this, SLOT(onClicked()));
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

ContactListItem::~ContactListItem()
{
    delete d;
}

QRectF ContactListItem::boundingRect() const
{
    return QRectF(4.0, 0.0, 480, 48);
}

void ContactListItem::setName(const QString &name)
{
    d->mName = name;
    update();
}

void ContactListItem::setStatusMessage(const QString &status)
{
    d->mStatusMessage = status;
    update();
}

void ContactListItem::setID(const QString &id)
{
    d->mID = id;
}

QString ContactListItem::name() const
{
    return d->mName;
}

QString ContactListItem::id()
{
    return d->mID;
}

QString ContactListItem::statusMessage() const
{
    return d->mStatusMessage;
}

void ContactListItem::setPixmap(const QPixmap &pixmap)
{
    d->mPixmap = pixmap;
    update();
}

void ContactListItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    PlexyDesk::StyleFeatures feature;
    feature.exposeRect = QRectF(option->exposedRect.bottomRight().x() - 2, option->exposedRect.bottomRight().y() -2,
                                option->exposedRect.bottomLeft().x() - 2 , option->exposedRect.bottomLeft().y() - 2);
    feature.state = PlexyDesk::StyleFeatures::SF_FrontView;

    d->mStyle->paintControlElement(PlexyDesk::Style::CE_Seperator, feature, painter);

    QPen pen(QColor(88, 88, 88), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen shadowPen(QColor(255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    QPainterPath shadowPath;
    float offset = 8.0;
    float radius = 6.0;

    float avatarHeight = (option->exposedRect.height() -  (2 * offset));
    float avatarWidth = avatarHeight;

    if (d->mAvatar.isNull()) {
        d->mAvatar = d->genShadowImage(QRect(0, 0, avatarWidth, avatarHeight), shadowPath, d->mPixmap);
    }

    shadowPath.addRoundedRect(QRectF (0.0, 0.0, avatarWidth , avatarHeight), radius, radius);
    painter->drawPixmap(QRect(offset, offset, avatarWidth,  avatarHeight), d->mAvatar);

    QFont font = QFont("Georgia", 14);
    QFontMetrics matrix (font);
    float nameHeight = matrix.height();
   // float statusLabelHeight = (option->exposedRect.height() / 2) - (nameHeight / 2);
    painter->setFont(font);
    QRectF nameRect = QRectF ((offset + avatarWidth + 10), option->exposedRect.center().y() - (nameHeight / 2),
                              option->exposedRect.width(), option->exposedRect.height() / 2);


    painter->setPen(shadowPen);
    painter->drawText(nameRect.adjusted(1, 1, 1, 1), Qt::AlignVCenter, d->mName);
    painter->setOpacity(0.9);
    painter->setPen(pen);
    painter->drawText(nameRect, Qt::AlignVCenter, d->mName);

}

void ContactListItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void ContactListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO ;
    event->accept();
    Q_EMIT clicked(this);
}

QPixmap ContactListItem::PrivateContactListItem::genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap)
{
    QPixmap canvasSource (rect.size());

    QPainter painter;
    painter.begin(&canvasSource);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rect, Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillPath(path, QColor (220, 220, 220));
    painter.setClipPath(path);

    QPainterPath avatarPath;
    avatarPath.addRoundedRect(2.0, 2.0, rect.width() - 4.0, rect.height() - 4.0, 6.0, 6.0);
    painter.setClipPath(avatarPath);
    painter.drawPixmap(0.0, 0.0, rect.width(), rect.height(), pixmap);

    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(QBrush(Qt::white), 0.01));
    painter.setOpacity(0.20);
    painter.drawEllipse(1, - (rect.height() / 2), rect.width() * 2, (rect.height()) - 2);
    painter.end();

    return canvasSource;
}


void ContactListItem::onClicked()
{
    Q_EMIT clicked (this);
}
