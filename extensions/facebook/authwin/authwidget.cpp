/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include <plexy.h>
#include <plexyconfig.h>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>
#include <QXmlQuery>
#include <QXmlSerializer>
#include <QColor>
#include <QPalette>
#include "facebookdata.h"
#include "authwidget.h"
#include <controllerinterface.h>
#include <abstractdesktopview.h>

namespace PlexyDesk {

AuthWidget::AuthWidget(const QRectF &rect) :
    PlexyDesk::DesktopWidget(rect)
{
    mJsonHandle = new JsonHandler();
    const QString token = tokenFromConfig();
    mLoggedIn = false;
    mNtManager = new QNetworkAccessManager(this);
    mCookie = new QNetworkCookieJar(this);
    mNtManager->setCookieJar(mCookie);
    QRect webrect = QRect(10.0, 10.0, rect.width()-65, rect.height()-8);
    mView = new QWebViewItem(webrect, this);
    if (mView->page()) {
        mView->page()->setNetworkAccessManager(mNtManager);
    }
    connect(mView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadeFinished(bool)));
    connect(mView, SIGNAL(loadProgress(int)), this, SLOT(onLoadProgress(int)));
    connect(mView, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
    connect(mView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(onUrlChanged(const QUrl &)));
    if (token.isEmpty()) {
        mView->setUrl(QUrl(QLatin1String("https://graph.facebook.com/oauth/authorize?client_" \
                     "id=170356722999159&redirect_uri=http://www.facebook.com/connect" \
                     "/login_success.html&type=user_agent&display=popup&scope=manage_pages,read_stream&response_type=token")));
    } else {
        this->setVisible(false);
        QNetworkRequest request;
        request.setUrl(QUrl("https://graph.facebook.com/19292868552_118464504835613?access_token="+token));
        mReply = mNtManager->get(request);
        connect(mReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
    mProgressValue = 10;

    enableDefaultBackground(false);

    mShadowEffect = new QGraphicsDropShadowEffect(this);
    mShadowEffect->setBlurRadius(32);
    mShadowEffect->setXOffset(0);
    mShadowEffect->setYOffset(0);
    mShadowEffect->setColor(QColor(0.0, 0.0, 0.0));
    this->setGraphicsEffect(mShadowEffect);

    mBlurEffect = new QGraphicsBlurEffect(this);
    //mBlurEffect->setBlurRadius(1.3);
    mBlurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);

    mBlurAnimation =  new QPropertyAnimation(mBlurEffect, "blurRadius",this);
    mBlurAnimation->setDuration(300);
    mBlurAnimation->setStartValue(5.3);
    mBlurAnimation->setEndValue(1.3);
    mView->setGraphicsEffect(mBlurEffect);
}

AuthWidget::~AuthWidget()
{
    delete mJsonHandle;
}

void AuthWidget::paintFrontView(QPainter *painter, const QRectF &r)
{
    QRect bannerRect(r.x(),
         r.y(),
         r.width(),
         34);

    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(r, 8, 8);
    painter->fillPath(backgroundPath, QColor(255, 255, 255));
    painter->fillRect(bannerRect, QColor(109, 132, 180));

    /* progress bar */
    QRadialGradient gradient(50, 50, 50, 50, 50);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 1, 1));
    const QBrush brush(gradient);
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(4);
    pen.setBrush(QColor(157, 172, 241));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    int pfWidth = boundingRect().width() - 60;
    int pfHeight = 20;
    QRect progressFrame (boundingRect().x() + 10,
         38, mProgressValue * 2, pfHeight);
    painter->setPen(pen);
    QPoint start (progressFrame.x() + 2, progressFrame.y() + 10);
    QPoint end ((mProgressValue * 2) + 2, progressFrame.y() + 10);

    /* Facebook Text */
    if (not mLoggedIn) {
        QPoint start (progressFrame.x() + 2, progressFrame.y() + 10);
        QPoint end ((mProgressValue * 2) + 2, progressFrame.y() + 10);
        painter->drawLine(start, end);
        pen.setBrush(QColor(255, 255, 255));
        painter->setPen(pen);
        QRect txtFrame(progressFrame.x(), 10, r.width(), 40);
        painter->drawText(txtFrame,
             QString("Loading Facebook : %1%").arg(mProgressValue));
    }

    PlexyDesk::DesktopWidget::paintFrontView(painter, r);
}

void AuthWidget::revokeSession()
{
}

void AuthWidget::onDataReady()
{
}

void AuthWidget::onLoadeFinished(bool ok)
{
    if (ok) {
        mView->setVisible(true);
        update();
        mBlurAnimation->start();
    } else {
        mView->setUrl(QUrl(QLatin1String("https://graph.facebook.com/oauth/authorize?client_" \
                     "id=170356722999159&redirect_uri=http://www.facebook.com/connect" \
                     "/login_success.html&type=user_agent&display=popup")));
        mLoggedIn = false;
        update();
        mView->setVisible(true);

    }
    mProgressValue = 10;
}

void AuthWidget::onLoadProgress(int progress)
{
    mProgressValue = progress;
    update();
}

void AuthWidget::onLoadStarted()
{
    if (mView->isVisible()) {
        mView->setVisible(false);
    }
}

void AuthWidget::onUrlChanged(const QUrl &url)
{
    qDebug() << Q_FUNC_INFO << url;
    QString stringUrl = url.toString().replace("#", "?");
    QUrl fburl(stringUrl);
    qDebug() <<  fburl.queryItemValue("access_token");
    if (not fburl.queryItemValue("access_token").isEmpty()) {
        configState(AbstractDesktopWidget::DOCKED);
        this->setVisible(false);
        /* save the auth token */

        if(controller() && controller()->viewport()) {
            controller()->viewport()->sessionDataForController(controller()->controllerName(), "access_token", fburl.queryItemValue("access_token"));
        }

    }
}
void AuthWidget::readConfig(QString &user,
     QString &pass)
{
}

void AuthWidget::data(QVariantMap &data)
{
    qDebug() << Q_FUNC_INFO << data["data"];
}

void AuthWidget::onReadyRead()
{
    JsonData result = mJsonHandle->property(mReply->readAll(), "data");
    if (result.type() == JsonData::Error) {
        this->setVisible(true);
        configState(AbstractDesktopWidget::VIEW);
        mView->setVisible(false);
        mView->setUrl(QUrl(QLatin1String("https://graph.facebook.com/oauth/authorize?client_" \
                     "id=170356722999159&redirect_uri=http://www.facebook.com/connect" \
                     "/login_success.html&type=user_agent&display=popup")));

    }
}

QString AuthWidget::tokenFromConfig() const
{
    PlexyDesk::Config *config = PlexyDesk::Config::getInstance();
    QSettings *settings = config->coreSettings();;
    settings->beginGroup("facebook_plugin");
    QString token = settings->value("access_token").toString();
    settings->endGroup();
    qDebug() << Q_FUNC_INFO << token;
    return token;
}
}
