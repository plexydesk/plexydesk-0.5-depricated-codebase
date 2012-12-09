#include "facebookcontactcard.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGraphicsPixmapItem>

class FacebookContactCard::PrivateFacebookContactCard
{
public:

    PrivateFacebookContactCard(){}
    ~PrivateFacebookContactCard(){}

    QImage genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap);

    PlexyDesk::DataSource *mDataSource;
    QString mFirstName;
    QString mLastName;
    QString mPicture;
    QString mHomeTown;
    QString mLocation;
    QPixmap mUserPicture;
    QPixmap mCoverPicture;
    QNetworkAccessManager *mNtManager;
    QString mID;
    // ui
    QGraphicsPixmapItem *mUiPixmap;
};

FacebookContactCard::FacebookContactCard(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent), d (new PrivateFacebookContactCard)
{
    d->mDataSource  = 0;
    d->mNtManager = new QNetworkAccessManager(this);
    d->mUiPixmap = 0;
}

FacebookContactCard::~FacebookContactCard()
{
}

void FacebookContactCard::setDataSource(const QString &id, const QString &token, PlexyDesk::DataSource *dataSource)
{
    if (dataSource) {
        connect(dataSource, SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onDataUpdated(QVariantMap)));

        QVariantMap request;
        QVariant arg;
        request["command"] = QVariant("user");
        request["id"] = id;
        request["token"] = token;
        arg = request;
        d->mID = id;
        dataSource->setArguments(arg);
    }
}

void FacebookContactCard::onDataUpdated(QVariantMap map)
{
    if (map["command"] == "userdata" && map["id"] == d->mID) {
        qDebug() << Q_FUNC_INFO << map["first_name"];
        d->mFirstName = map["first_name"].toString();
        d->mLastName = map["last_name"].toString();
        d->mPicture = map["picture"].toString();
        d->mHomeTown = map["hometown"].toString();
        d->mLocation = map["location"].toString();

        QUrl url (map["cover"].toString());
        QNetworkReply *reply = d->mNtManager->get(QNetworkRequest(url));

        connect(reply, SIGNAL(finished()), this, SLOT(onCoverReady()));

        setLabelName(d->mFirstName);

    }
}

void FacebookContactCard::onCoverReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());


        if (reply) {

            if (reply->error() != QNetworkReply::NoError) {
                  qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                  return;
              }

            QString header = reply->header(QNetworkRequest::ContentTypeHeader).toString();

            const QByteArray data = reply->readAll();
            int bufferSize = reply->bytesAvailable();

            QPixmap pixmap;
            pixmap.loadFromData(data);

            if(pixmap.isNull()) {
                qDebug() << Q_FUNC_INFO << "Null Image data from : " << reply->url() << " : was finished ?" << reply->isFinished();
            } else {
                d->mCoverPicture = pixmap;
                update();
                qDebug() << Q_FUNC_INFO << "cover";
                qDebug() << Q_FUNC_INFO << d->mPicture;

                QNetworkReply *reply_img = d->mNtManager->get(QNetworkRequest(QUrl(d->mPicture)));

                connect(reply_img, SIGNAL(finished()), this, SLOT(onAvatarReady()));
            }
        }

        reply->deleteLater();
    }
}

void FacebookContactCard::onAvatarReady()
{
    if (sender()) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*> (sender());


        if (reply) {

            if (reply->error() != QNetworkReply::NoError) {
                  qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                  return;
              }

            QString header = reply->header(QNetworkRequest::ContentTypeHeader).toString();

            const QByteArray data = reply->readAll();
            int bufferSize = reply->bytesAvailable();

            QPixmap pixmap;
            pixmap.loadFromData(data);

            if(pixmap.isNull()) {
                qDebug() << Q_FUNC_INFO << "Null Image data from : " << reply->url() << " : was finished ?" << reply->isFinished();
            } else {
                d->mUserPicture = pixmap;
                update();
                qDebug() << Q_FUNC_INFO << "avatar";
            }
        }

        reply->deleteLater();
    }
}

QImage FacebookContactCard::PrivateFacebookContactCard::genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap)
{
    QImage canvasSource (rect.size(), QImage::Format_ARGB32_Premultiplied);

    if (pixmap.isNull())
        return canvasSource;

    QPainter painter;
    painter.begin(&canvasSource);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(rect, Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.setClipPath(path);
    painter.fillPath(path, QColor (220, 220, 220));


    painter.drawPixmap(QRect(0.0, 0.0, rect.width(), rect.height()), pixmap,
                       QRect(0.0, 0.0, rect.width(), rect.height()));

    //QPen pen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    //painter.drawEllipse(rect);

    const qreal rad = (rect.width() / 2 ) + 1;

    QRadialGradient gr(rad, rad, rad, 3 * rad / 5, 3 * rad / 5);
    gr.setColorAt(0.0, QColor(255, 255, 255, 0));
    gr.setColorAt(0.2, QColor(255, 255, 255, 0));
    gr.setColorAt(0.9, QColor(150, 150, 200, 63));
    gr.setColorAt(0.95, QColor(0, 0, 0, 127));
    gr.setColorAt(1, QColor(0, 0, 0, 0));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(gr);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(0.9);
    painter.drawEllipse(rect);

    return canvasSource;
}

void FacebookContactCard::paintFrontView(QPainter *painter, const QRectF &rect)
{
    QRectF pixmapRect = d->mCoverPicture.rect();

    /* Draw background */
    QPainterPath parentPath;
    parentPath.addRoundedRect(rect.x(), rect.y(), rect.width(), rect.height(), 8.0, 8.0);
    painter->setClipPath(parentPath);
    painter->fillPath(parentPath, QColor(255, 255, 255));

    /*calculate the new height for the content rect width */
    float pixmapHeight = (pixmapRect.height() / pixmapRect.width() ) * contentRect().width();
    pixmapRect.setHeight(pixmapHeight);
    pixmapRect.setWidth(contentRect().width());
    painter->drawPixmap(QRectF(rect.x(), rect.y(), rect.width(), pixmapRect.height()), d->mCoverPicture,
                        QRectF(pixmapRect.x(), pixmapRect.y(), d->mCoverPicture.width(), d->mCoverPicture.height()));

    /* Draw Avatar */
    QPainterPath shadowPath;
    float offset = pixmapRect.height() - 60;
    float radius = 6.0;
    float avatarHeight =  d->mUserPicture.width();
    float avatarWidth = d->mUserPicture.width();

    if (d->mUserPicture.width() > d->mUserPicture.height()) {
        avatarWidth = d->mUserPicture.height();
        avatarHeight = d->mUserPicture.height();
    }

    if (d->mUserPicture.width() < d->mUserPicture.height())  {
        avatarWidth = d->mUserPicture.width();
        avatarHeight = d->mUserPicture.width();
    }

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPainterPath ringPath;
    ringPath.addEllipse((QRectF(6, offset - 4, avatarWidth + 8, avatarHeight + 8)));
    painter->fillPath(ringPath, QColor(255, 255, 255));

    painter->restore();

    //shadowPath.addoundedRect(QRectF (0.0, 0.0, avatarWidth , avatarHeight), radius, radius);
    shadowPath.addEllipse(QRectF(0.0, 0.0, avatarWidth, avatarHeight));
    painter->drawImage(QRectF(10, offset, avatarWidth, avatarHeight),
                       d->genShadowImage(QRect(0, 0, avatarWidth, avatarHeight), shadowPath, d->mUserPicture));

    QRectF detailBackgroundRect = QRectF(avatarWidth + 12, pixmapRect.height(), rect.width() - avatarWidth + 12, rect.height());
    painter->fillRect(detailBackgroundRect, QColor(245, 245, 245));

    /* Draw Text */
    QPen pen(QColor(77, 77, 77), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont font = QFont ("", 16);
    painter->setFont(font);
    painter->setPen(pen);

    int padding = 10;
    QRectF nameRect =
            QRectF(detailBackgroundRect.x() + padding, detailBackgroundRect.y() + padding, detailBackgroundRect.width(), 100);

    painter->drawText(nameRect,  Qt::AlignLeft, d->mFirstName + " " + d->mLastName);

    QRectF hometownRect =
            QRectF(detailBackgroundRect.x() + padding, detailBackgroundRect.y() + padding + 20, detailBackgroundRect.width(), 100);

    font = QFont ("", 10);
    painter->setFont(font);
    painter->drawText(hometownRect,  Qt::AlignLeft, d->mLocation);

}

void FacebookContactCard::paintDockView(QPainter *painter, const QRectF &rect)
{

    PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
    QPainterPath parentPath = painter->clipPath();
    painter->drawPixmap(rect.x(), rect.y(), rect.width(), rect.height(), d->mUserPicture);

    painter->setClipPath(parentPath);
}
