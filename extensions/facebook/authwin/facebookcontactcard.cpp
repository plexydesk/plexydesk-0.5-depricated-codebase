#include "facebookcontactcard.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGraphicsPixmapItem>
#include <style.h>
#include <nativestyle.h>
#include <abstractdesktopview.h>
#include <button.h>
#include "facebookcontactcardbutton.h"
#include "facebookmessagedialog.h"
#include <controllerinterface.h>
#include <webkitwebview.h>
#include <plexyconfig.h>

//
// Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
void fastblur(QImage &img, int radius)
{
    if (radius < 1) {
        return;
    }

    QRgb *pix = (QRgb*)img.bits();
    int w   = img.width();
    int h   = img.height();
    int wm  = w-1;
    int hm  = h-1;
    int wh  = w*h;
    int div = radius+radius+1;

    int *r = new int[wh];
    int *g = new int[wh];
    int *b = new int[wh];
    int rsum, gsum, bsum, x, y, i, p, yp, yi, yw;
    int *vmin = new int[qMax(w,h)];

    int divsum = (div+1)>>1;
    divsum *= divsum;
    int *dv = new int[256*divsum];
    for (i=0; i < 256*divsum; ++i) {
        dv[i] = (i/divsum);
    }

    yw = yi = 0;

    int **stack = new int*[div];
    for(int i = 0; i < div; ++i) {
        stack[i] = new int[3];
    }


    int stackpointer;
    int stackstart;
    int *sir;
    int rbs;
    int r1 = radius+1;
    int routsum,goutsum,boutsum;
    int rinsum,ginsum,binsum;

    for (y = 0; y < h; ++y){
        rinsum = ginsum = binsum = routsum = goutsum
               = boutsum = rsum = gsum = bsum = 0;
        for(i =- radius; i <= radius; ++i) {
            p = pix[yi+qMin(wm,qMax(i,0))];
            sir = stack[i+radius];
            sir[0] = (p & 0xff0000)>>16;
            sir[1] = (p & 0x00ff00)>>8;
            sir[2] = (p & 0x0000ff);
            rbs = r1-abs(i);
            rsum += sir[0]*rbs;
            gsum += sir[1]*rbs;
            bsum += sir[2]*rbs;
            if (i > 0){
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
            }
        }
        stackpointer = radius;

        for (x=0; x < w; ++x) {

            r[yi] = dv[rsum];
            g[yi] = dv[gsum];
            b[yi] = dv[bsum];

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];

            if (y == 0) {
                vmin[x] = qMin(x+radius+1,wm);
            }
            p = pix[yw+vmin[x]];

            sir[0] = (p & 0xff0000)>>16;
            sir[1] = (p & 0x00ff00)>>8;
            sir[2] = (p & 0x0000ff);

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[(stackpointer)%div];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];

            ++yi;
        }
        yw += w;
    }
    for (x=0; x < w; ++x){
        rinsum = ginsum = binsum = routsum = goutsum
               = boutsum = rsum = gsum = bsum=0;
        yp =- radius*w;
        for(i=-radius; i <= radius; ++i) {
            yi=qMax(0,yp)+x;

            sir = stack[i+radius];

            sir[0] = r[yi];
            sir[1] = g[yi];
            sir[2] = b[yi];

            rbs = r1-abs(i);

            rsum += r[yi]*rbs;
            gsum += g[yi]*rbs;
            bsum += b[yi]*rbs;

            if (i > 0) {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
            }

            if (i < hm){
                yp += w;
            }
        }

        yi = x;
        stackpointer = radius;

        for (y=0; y < h; ++y){
            pix[yi] = 0xff000000 | (dv[rsum]<<16) | (dv[gsum]<<8) | dv[bsum];

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];

            if (x==0){
                vmin[y] = qMin(y+r1,hm)*w;
            }
            p = x+vmin[y];

            sir[0] = r[p];
            sir[1] = g[p];
            sir[2] = b[p];

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[stackpointer];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];

            yi += w;
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;
    delete [] vmin;
    delete [] dv;

    for(int i = 0; i < div; ++i) {
        delete [] stack[i];
    }
    delete [] stack;
}



// Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
void fastbluralpha(QImage &img, int radius)
{
    if (radius < 1) {
        return;
    }

    QRgb *pix = (QRgb*)img.bits();
    int w   = img.width();
    int h   = img.height();
    int wm  = w-1;
    int hm  = h-1;
    int wh  = w*h;
    int div = radius+radius+1;

    int *r = new int[wh];
    int *g = new int[wh];
    int *b = new int[wh];
    int *a = new int[wh];
    int rsum, gsum, bsum, asum, x, y, i, yp, yi, yw;
    QRgb p;
    int *vmin = new int[qMax(w,h)];

    int divsum = (div+1)>>1;
    divsum *= divsum;
    int *dv = new int[256*divsum];
    for (i=0; i < 256*divsum; ++i) {
        dv[i] = (i/divsum);
    }

    yw = yi = 0;

    int **stack = new int*[div];
    for(int i = 0; i < div; ++i) {
        stack[i] = new int[4];
    }


    int stackpointer;
    int stackstart;
    int *sir;
    int rbs;
    int r1 = radius+1;
    int routsum, goutsum, boutsum, aoutsum;
    int rinsum, ginsum, binsum, ainsum;

    for (y = 0; y < h; ++y){
        rinsum = ginsum = binsum = ainsum
               = routsum = goutsum = boutsum = aoutsum
               = rsum = gsum = bsum = asum = 0;
        for(i =- radius; i <= radius; ++i) {
            p = pix[yi+qMin(wm,qMax(i,0))];
            sir = stack[i+radius];
            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rbs = r1-abs(i);
            rsum += sir[0]*rbs;
            gsum += sir[1]*rbs;
            bsum += sir[2]*rbs;
            asum += sir[3]*rbs;

            if (i > 0){
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }
        }
        stackpointer = radius;

        for (x=0; x < w; ++x) {

            r[yi] = dv[rsum];
            g[yi] = dv[gsum];
            b[yi] = dv[bsum];
            a[yi] = dv[asum];

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (y == 0) {
                vmin[x] = qMin(x+radius+1,wm);
            }
            p = pix[yw+vmin[x]];

            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[(stackpointer)%div];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            ++yi;
        }
        yw += w;
    }
    for (x=0; x < w; ++x){
        rinsum = ginsum = binsum = ainsum
               = routsum = goutsum = boutsum = aoutsum
               = rsum = gsum = bsum = asum = 0;

        yp =- radius * w;

        for(i=-radius; i <= radius; ++i) {
            yi=qMax(0,yp)+x;

            sir = stack[i+radius];

            sir[0] = r[yi];
            sir[1] = g[yi];
            sir[2] = b[yi];
            sir[3] = a[yi];

            rbs = r1-abs(i);

            rsum += r[yi]*rbs;
            gsum += g[yi]*rbs;
            bsum += b[yi]*rbs;
            asum += a[yi]*rbs;

            if (i > 0) {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }

            if (i < hm){
                yp += w;
            }
        }

        yi = x;
        stackpointer = radius;

        for (y=0; y < h; ++y){
            pix[yi] = qRgba(dv[rsum], dv[gsum], dv[bsum], dv[asum]);

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (x==0){
                vmin[y] = qMin(y+r1,hm)*w;
            }
            p = x+vmin[y];

            sir[0] = r[p];
            sir[1] = g[p];
            sir[2] = b[p];
            sir[3] = a[p];

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[stackpointer];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            yi += w;
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;
    delete [] a;
    delete [] vmin;
    delete [] dv;

    for(int i = 0; i < div; ++i) {
        delete [] stack[i];
    }
    delete [] stack;
}

//

class FacebookContactCard::PrivateFacebookContactCard
{
public:

    PrivateFacebookContactCard(){}
    ~PrivateFacebookContactCard()
    {
    }

    QImage genShadowImage(const QRect &rect, const QPainterPath &path, const QPixmap &pixmap);

    PlexyDesk::DataSource *mDataSource;
    QString mFirstName;
    QString mLastName;
    QString mPicture;
    QString mHomeTown;
    QString mLocation;
    QPixmap mUserPicture;
    QPixmap mCoverPicture;
    QString mStatusMessage;
    QNetworkAccessManager *mNtManager;
    QString mID;
    QString mToken;
    int mCoverOffset;
    // ui
    QGraphicsPixmapItem *mUiPixmap;
    PlexyDesk::Style *mStyle;
    FacebookContactCardButton *mWallPostButton;
    FacebookMessageDialog *mPostDialog;

    //feed
    PlexyDesk::WebKitWebView *mFeedWall;
    FacebookContactCardButton *mFeedButton;

};

FacebookContactCard::FacebookContactCard(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent), d (new PrivateFacebookContactCard)
{
    d->mDataSource  = 0;
    d->mNtManager = new QNetworkAccessManager(this);
    d->mUiPixmap = 0;
    d->mFeedWall = 0;
    d->mStyle = new PlexyDesk::NativeStyle(this);
    setDockRect(QRect(rect.x(), rect.y(), 72, 72));
    d->mWallPostButton = new FacebookContactCardButton(this);
    d->mWallPostButton->show();
    d->mWallPostButton->setLabel(tr("Send Wall Post"));
    d->mPostDialog = new FacebookMessageDialog(QRect(rect.x(), rect.y(), rect.width(), 200));
    d->mPostDialog->hide();

    //feed
    d->mFeedButton = new FacebookContactCardButton(this);
    d->mFeedButton->show();
    d->mFeedButton->setLabel(tr("View Feed"));

    connect(d->mWallPostButton, SIGNAL(clicked()), this, SLOT(onWallPostClicked()));
    connect(d->mFeedButton, SIGNAL(clicked()), this, SLOT(feedClicked()));
    connect(d->mPostDialog, SIGNAL(messageRequested()), this, SLOT(onMessageRestested()));
    connect(d->mPostDialog, SIGNAL(closed(PlexyDesk::AbstractDesktopWidget*)), this, SLOT(onWidgetClosed(PlexyDesk::AbstractDesktopWidget*)));
}

FacebookContactCard::~FacebookContactCard()
{
    qDebug() << Q_FUNC_INFO;
    delete d;
}

void FacebookContactCard::setDataSource(const QString &id, const QString &token, PlexyDesk::DataSource *dataSource)
{
    if (dataSource) {
        connect(dataSource, SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onDataUpdated(QVariantMap)));
        d->mDataSource = dataSource;

        QVariantMap request;
        QVariant arg;
        request["command"] = QVariant("user");
        request["id"] = id;
        request["token"] = token;
        arg = request;
        d->mID = id;
        d->mToken = token;
        dataSource->setArguments(arg);

        if (!d->mFeedWall) {
            d->mFeedWall = new PlexyDesk::WebKitWebView(boundingRect(), 0);
            d->mFeedWall->injectQObject("FacebookEngine", d->mDataSource);
            d->mFeedWall->injectQObject("ContactCard", this);

            if (controller()) {
                PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(controller()->viewport());

                if (view)
                    view->addWidgetToView(d->mFeedWall);
            }
            d->mFeedWall->hide();

            connect(d->mFeedWall, SIGNAL(closed(PlexyDesk::AbstractDesktopWidget*)), this, SLOT(onFeedClosed(PlexyDesk::AbstractDesktopWidget*)));

            QString prefix = PlexyDesk::Config::getInstance()->plexydeskBasePath();

            QUrl url ("file://" + prefix + "/" + "share/plexy/facebook/ui/data/" + "index.html");

            d->mFeedWall->setUrl(url);
        }
    }
}

QString FacebookContactCard::facebookId() const
{
    return d->mID;
}

QPixmap FacebookContactCard::userImage() const
{
   return d->mUserPicture;
}

QPixmap FacebookContactCard::coverImage() const
{
    return d->mCoverPicture;
}

void FacebookContactCard::onDataUpdated(QVariantMap map)
{
    if (map["command"] == "userdata" && map["id"] == d->mID) {
        d->mFirstName = map["first_name"].toString();
        d->mLastName = map["last_name"].toString();
        d->mPicture = map["picture"].toString();
        d->mHomeTown = map["hometown"].toString();
        d->mLocation = map["location"].toString();
        d->mCoverOffset = map["cover_offset"].toInt();

        this->requestStatusMessage();

        QUrl url (map["cover"].toString());
        QNetworkReply *reply = d->mNtManager->get(QNetworkRequest(url));

        connect(reply, SIGNAL(finished()), this, SLOT(onCoverReady()));

        setLabelName(d->mID);
        QString buttonLabel = "Write on " + d->mFirstName + "'s Wall";
        QString buttonLabel2 = d->mFirstName + "'s Feed";
        d->mWallPostButton->setLabel(buttonLabel);
        d->mFeedButton->setLabel(buttonLabel2);
    }
}

void FacebookContactCard::onStatusUpdated(QVariantMap map)
{
    if (map["command"].toString() == "status" && map["id"].toString() == d->mID) {
        d->mStatusMessage = map["message"].toString();
        update();
    }
}

void FacebookContactCard::onCoverReady()
{
    QNetworkReply *reply_img = d->mNtManager->get(QNetworkRequest(QUrl(d->mPicture)));
    connect(reply_img, SIGNAL(finished()), this, SLOT(onAvatarReady()));

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

            QImage img;
            img.loadFromData(data);
            fastbluralpha(img, 18.0);
            pixmap = QPixmap::fromImage(img);

            if(pixmap.isNull()) {
                qDebug() << Q_FUNC_INFO << "Null Image data from : " << reply->url() << " : was finished ?" << reply->isFinished();
            } else {
                d->mCoverPicture = pixmap;
                update();
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
            }
        }

        reply->deleteLater();
    }
}

void FacebookContactCard::onWallPostClicked()
{
    if (this->controller() && this->controller()->viewport() && d->mPostDialog) {

       PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(controller()->viewport());
       if (view)
           view->addWidgetToView(d->mPostDialog);

        d->mPostDialog->setPos(this->mapToScene(d->mWallPostButton->pos()));
    }
}

void FacebookContactCard::feedClicked()
{
    qDebug() << Q_FUNC_INFO;

    if (d->mFeedWall) {
        if (controller()) {
            PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(controller()->viewport());
            if (view)
               view->addWidgetToView(d->mFeedWall);
        }
        d->mFeedWall->show();
    } else {
        d->mFeedWall = new PlexyDesk::WebKitWebView(boundingRect(), 0);
        d->mFeedWall->injectQObject("FacebookEngine", d->mDataSource);
        d->mFeedWall->injectQObject("ContactCard", this);
        connect(d->mFeedWall, SIGNAL(closed(PlexyDesk::AbstractDesktopWidget*)), this, SLOT(onFeedClosed(PlexyDesk::AbstractDesktopWidget*)));

        if (controller()) {
            PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(controller()->viewport());
            if (view)
                view->addWidgetToView(d->mFeedWall);
        }


        QString prefix = PlexyDesk::Config::getInstance()->plexydeskBasePath();

        QUrl url ("file://" + prefix + "/" + "share/plexy/facebook/ui/data/" + "index.html");

        qDebug() << Q_FUNC_INFO << url;
        d->mFeedWall->setUrl(url);
    }
}

void FacebookContactCard::onMessageRestested()
{
    if (d->mPostDialog) {
        QVariantMap request;
        QVariant arg;

        request["command"] = QVariant("wallpost");
        request["id"] = d->mID;
        request["token"] = d->mToken;
        request["message"] = d->mPostDialog->message();

        arg = request;

        d->mDataSource->setArguments(arg);
    }
}

void FacebookContactCard::onWidgetClosed(PlexyDesk::AbstractDesktopWidget *)
{
    d->mPostDialog = new FacebookMessageDialog(QRect(boundingRect().x(), boundingRect().y(), boundingRect().width(), 200));
    d->mPostDialog->hide();
    connect(d->mPostDialog, SIGNAL(messageRequested()), this, SLOT(onMessageRestested()));
    connect(d->mPostDialog, SIGNAL(closed(PlexyDesk::AbstractDesktopWidget*)), this, SLOT(onWidgetClosed(PlexyDesk::AbstractDesktopWidget*)));
}

void FacebookContactCard::onFeedClosed(PlexyDesk::AbstractDesktopWidget *)
{
    qDebug() << Q_FUNC_INFO ;
    d->mFeedWall = 0;
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


    painter.drawPixmap(QRect(rect.x(), rect.y(), rect.width(), rect.height()), pixmap,
                       QRect(0.0, 0.0, rect.width(), rect.height()));

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
    float pixmapHeight = (pixmapRect.height() / pixmapRect.width()) * contentRect().width();
    float coverYOffset = 0;

    if (d->mCoverOffset > 0) {
        coverYOffset = (pixmapRect.height() / pixmapRect.width()) * d->mCoverOffset;
    }

    pixmapRect.setHeight(pixmapHeight - d->mCoverOffset);
    pixmapRect.setWidth(contentRect().width());
    painter->fillRect (QRectF(rect.x(), rect.y(), rect.width(), 128), QColor(65, 53, 50));
    painter->drawPixmap(QRectF(rect.x(), rect.y(), rect.width(), 128), d->mCoverPicture,
                        QRectF(0.0, coverYOffset, d->mCoverPicture.width(), d->mCoverPicture.height() - d->mCoverOffset));

    /* Draw Avatar */
    QPainterPath shadowPath;
    float offset = 128 - 60;

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

    shadowPath.addEllipse(QRectF(0.0, 0.0, avatarWidth, avatarHeight));
    painter->drawImage(QRectF(10, offset, avatarWidth, avatarHeight),
                       d->genShadowImage(QRect(0, 0, avatarWidth, avatarHeight), shadowPath, d->mUserPicture));

    QRectF detailBackgroundRect = QRectF(avatarWidth + 12, 128, rect.width() - avatarWidth + 12, rect.height());
    painter->fillRect(detailBackgroundRect, QColor(245, 245, 245));

    /* Draw Text */
    QPen pen(QColor(77, 77, 77), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    PlexyDesk::StyleFeatures feature;
    feature.state = PlexyDesk::StyleFeatures::SF_Raised;
    feature.fontColor = QColor(77, 77, 77);
    feature.fontPen = pen;
    feature.fontFlags = Qt::AlignLeft;
    int padding = 10;
    QRectF nameRect =
            QRectF(detailBackgroundRect.x() + padding, detailBackgroundRect.y() + padding, detailBackgroundRect.width(), 100);
    QRectF hometownRect =
            QRectF(detailBackgroundRect.x() + padding, detailBackgroundRect.y() + padding + 20, detailBackgroundRect.width(), 100);
    QRectF statusMessageRect =
            QRectF(detailBackgroundRect.x(), 10, detailBackgroundRect.width() - 30, 118);

    if(d->mStyle) {
        feature.font = QFont ("Georgia", 16);
        feature.exposeRect = nameRect;
        d->mStyle->paintControlElementText(PlexyDesk::Style::CE_Label, feature, tr(d->mFirstName.toUtf8()) + " " + tr(d->mLastName.toUtf8()), painter);

        feature.font = QFont ("Georgia", 10);
        feature.exposeRect = hometownRect;
        d->mStyle->paintControlElementText(PlexyDesk::Style::CE_Label, feature, tr("Lives in") + "  " + d->mLocation, painter);

        if (!d->mStatusMessage.isEmpty()) {
            feature.font = QFont ("Georgia", 11, QFont::Bold, false);
            feature.exposeRect = statusMessageRect;
            feature.fontColor = QColor(110, 110, 110);
            feature.fontPen = pen;
            feature.fontFlags = Qt::AlignLeft | Qt::TextWordWrap;
            d->mStyle->paintControlElementText(PlexyDesk::Style::CE_Label, feature, "\"" +d->mStatusMessage + "\"", painter);
        }

    }

    if (d->mWallPostButton) {
        QRectF wallpostButtonRect =
                QRectF(detailBackgroundRect.x() + padding, detailBackgroundRect.y() + padding + 40, detailBackgroundRect.width() - 40, 24);
        d->mWallPostButton->setSize(QSize(wallpostButtonRect.width(), wallpostButtonRect.height()));
        d->mWallPostButton->setPos(wallpostButtonRect.topLeft());
    }

    if (d->mFeedButton) {
        QRectF wallpostButtonRect =
                QRectF(detailBackgroundRect.x() + padding, detailBackgroundRect.y() + padding + 80, detailBackgroundRect.width() - 40, 24);
        d->mFeedButton->setSize(QSize(wallpostButtonRect.width(), wallpostButtonRect.height()));
        d->mFeedButton->setPos(wallpostButtonRect.topLeft());
    }
}

void FacebookContactCard::paintDockView(QPainter *painter, const QRectF &rect)
{
    //PlexyDesk::DesktopWidget::paintDockView(painter, rect);

    /* Draw Avatar */
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

    QPainterPath mainPath;
    mainPath.addRoundRect(rect, 8.0);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);


    painter->setClipPath(mainPath);
    painter->drawPixmap(QRect(rect.x(), rect.y(), rect.width(), rect.height()), d->mUserPicture,
                        QRect(0, 0, dockRect().width() - 4  , dockRect().height() - 4));

    QPainterPath framePath;
    framePath.addRoundRect(rect.adjusted(1.0, 1.0, -1.0 , -1.0), 8.0);
    QPen pen = QPen(QColor (245, 245, 245), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->setOpacity(0.8);
    painter->drawPath(framePath);
    //painter->fillPath(framePath, QColor(255, 255, 255));
}

void FacebookContactCard::requestStatusMessage()
{
    if (d->mDataSource) {
        QVariantMap request;
        QVariant arg;

        request["command"] = QVariant("status");
        request["id"] = d->mID;
        request["token"] = d->mToken;

        arg = request;

        connect(d->mDataSource, SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onStatusUpdated(QVariantMap)));
        d->mDataSource->setArguments(arg);
    }
}
