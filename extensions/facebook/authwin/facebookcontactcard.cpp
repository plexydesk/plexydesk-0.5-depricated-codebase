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
    painter.fillPath(path, QColor (220, 22, 220));
    painter.setClipPath(path);

    QPainterPath avatarPath;
    avatarPath.addRoundedRect(2.0, 2.0, rect.width() , rect.height(), 16.0, 16.0);
    painter.setClipPath(avatarPath);
    float pixmapHeight = (pixmap.height() / pixmap.width() ) * rect.width();

    if (pixmapHeight <= 0)
        pixmapHeight = rect.height();

    painter.drawPixmap(QRect(0.0, 0.0, rect.width(), pixmapHeight), pixmap,
                       QRect(0.0, 0.0, pixmap.width(), pixmap.height()));

    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(QBrush(Qt::white), 0.01));
    painter.setOpacity(0.20);
    painter.drawEllipse(1, - (rect.height() / 2), rect.width() * 2, (rect.height()) - 2);
    painter.end();

    return canvasSource;
}

void FacebookContactCard::paintFrontView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
    QPainterPath parentPath = painter->clipPath();
    QRectF pixmapRect = d->mCoverPicture.rect();

    /*calculate the new height for the content rect width */
    float pixmapHeight = (pixmapRect.height() / pixmapRect.width() ) * contentRect().width();
    pixmapRect.setHeight(pixmapHeight);
    pixmapRect.setWidth(contentRect().width());
    painter->drawPixmap(QRectF(pixmapRect.x(), pixmapRect.y(), pixmapRect.width(), pixmapRect.height()), d->mCoverPicture,
                        QRectF(pixmapRect.x(), pixmapRect.y(), d->mCoverPicture.width(), d->mCoverPicture.height()));

    painter->save();

    painter->setOpacity(0.8);
    QPainterPath backgroundPath;
    backgroundPath.addRoundRect(QRect(0, pixmapRect.height() - 40, rect.width(), 40), 8);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , pixmapRect.height() - 40));

    linearGrad.setColorAt(0, QColor(189, 191, 196));
    linearGrad.setColorAt(1, QColor(255, 255, 255));

    painter->fillPath(backgroundPath, linearGrad);

    painter->restore();

    QPen pen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont font = QFont ("", 20);
    painter->setFont(font);
    painter->setPen(pen);
    painter->drawText(QRect((d->mUserPicture.width() / 2 ) - 10, pixmapRect.height() - 40, rect.width(), 40),  Qt::AlignLeft | Qt::AlignHCenter, d->mFirstName);
    QPainterPath shadowPath;
    float offset = pixmapRect.height() - 60;
    float radius = 6.0;

    float avatarHeight =  d->mUserPicture.height();
    float avatarWidth = d->mUserPicture.width();

    shadowPath.addRoundedRect(QRectF (0.0, 0.0, avatarWidth , avatarHeight), radius, radius);
    painter->drawImage(QRectF(10, offset, avatarWidth,  avatarHeight),
                       d->genShadowImage(QRect(0, 0, avatarWidth, avatarHeight), shadowPath, d->mUserPicture));

    painter->setClipPath(parentPath);

}

void FacebookContactCard::paintDockView(QPainter *painter, const QRectF &rect)
{

    PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
    QPainterPath parentPath = painter->clipPath();
    painter->drawPixmap(rect.x(), rect.y(), rect.width(), rect.height(), d->mUserPicture);

    painter->setClipPath(parentPath);

}
