#include "contactlistitem.h"
#include <nativestyle.h>
#include <QStyleOptionGraphicsItem>
#include <blitz/qimageblitz.h>

class ContactListItem::PrivateContactListItem {
public:
    PrivateContactListItem() {}
    ~PrivateContactListItem() {}

    QImage genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap);

    PlexyDesk::Style *mStyle;
    QString mName;
    QString mStatusMessage;
    QPixmap mPixmap;

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
    return QRectF(4.0, 0.0, 480, 60);
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

QString ContactListItem::name() const
{
    return d->mName;
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
    feature.exposeRect = option->exposedRect;
    feature.state = PlexyDesk::StyleFeatures::SF_FrontView;

    d->mStyle->paintControlElement(PlexyDesk::Style::CE_Frame, feature, painter);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPen pen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

//    feature.exposeRect = QRectF(4.0,0.0, option->exposedRect.width(), 1.0);
//    d->mStyle->paintControlElement(PlexyDesk::Style::CE_Seperator, feature, painter);

    QPainterPath shadowPath;
    float offset = 8.0;
    float radius = 6.0;

    float avatarHeight = (option->exposedRect.height() -  (2 * offset));
    float avatarWidth = avatarHeight;

    shadowPath.addRoundedRect(QRectF (0.0, 0.0, avatarWidth , avatarHeight), radius, radius);
    painter->drawImage(QRectF(offset, offset, avatarWidth,  avatarHeight),
                       d->genShadowImage(QRect(0, 0, avatarWidth, avatarHeight), shadowPath, d->mPixmap));

    QFont font = QFont("", 16);
    QFontMetrics matrix (font);
    float nameHeight = matrix.height();
    float statusLabelHeight = (option->exposedRect.height() / 2) - (nameHeight / 2);
    painter->setFont(font);
    painter->setPen(pen);
    QRectF nameRect = QRectF ((offset + avatarWidth + 10), 4.0, option->exposedRect.width(), option->exposedRect.height() / 2);
    painter->drawText(nameRect, Qt::AlignVCenter, d->mName);

    font = QFont ("", 12);
    QFontMetrics statusMetrix (font);
    nameHeight = statusMetrix.height();
    statusLabelHeight = (option->exposedRect.height() / 2) - (nameHeight / 2);
    painter->setFont(font);
    painter->setPen(pen);
    nameRect = QRectF ((offset + avatarWidth + 10), nameRect.height() + 4.0, option->exposedRect.width(), option->exposedRect.height() / 2);
    QString statusText = statusMessage();
    if (statusText.size() > 70) {
        statusText.truncate(70);
        statusText += "...";
    }
    painter->drawText(nameRect, Qt::AlignVCenter, statusText);
}

void ContactListItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void ContactListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}


QImage ContactListItem::PrivateContactListItem::genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap)
{
    QImage canvasSource (rect.size(), QImage::Format_ARGB32_Premultiplied);

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

    //return Blitz::gaussianBlur(canvasSource, 1, 1);
    return canvasSource;
}
