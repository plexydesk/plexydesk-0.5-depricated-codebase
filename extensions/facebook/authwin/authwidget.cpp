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

namespace PlexyDesk{

AuthWidget::AuthWidget(const QRectF &rect, QWidget *widget):
        PlexyDesk::DesktopWidget(rect, widget)
{
    mJsonHandle = new JsonHandler();
    const QString token = tokenFromConfig();
    mLoggedIn = false;
    mNtManager = new QNetworkAccessManager(this);
    mCookie = new QNetworkCookieJar(this);
    mNtManager->setCookieJar(mCookie);
    QRect webrect = QRect(10.0, 10.0, rect.width()-65, rect.height()-5);
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
                    "/login_success.html&type=user_agent&display=popup")));
    } else {
        this->configState(DesktopWidget::DOCK);
        this->setVisible(false);
        QNetworkRequest request;
        request.setUrl(QUrl("https://graph.facebook.com/19292868552_118464504835613?access_token="+token));
        mReply = mNtManager->get(request);
        connect(mReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }
    mProgressValue = 10;
}

AuthWidget::~AuthWidget()
{
    delete mJsonHandle;
}

void AuthWidget::paintExtFace(QPainter *painter,
        const QStyleOptionGraphicsItem *op, QWidget *)
{
    QRect bannerRect(op->exposedRect.x(),
            op->exposedRect.y(),
            op->exposedRect.width(),
            36);
    painter->fillRect(op->exposedRect, QColor(255, 255, 255));
    painter->fillRect(bannerRect, QColor(109, 132, 180));
    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

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
           38 , mProgressValue * 2, pfHeight);
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
        QRect txtFrame(progressFrame.x(), 10, op->exposedRect.width(), 40);
        painter->drawText(txtFrame,
                QString("Loading Facebook : %1%").arg(mProgressValue));
    }
}
void AuthWidget::paintExtDockFace(QPainter *painter,
        const QStyleOptionGraphicsItem *, QWidget *)
{
    mView->setVisible(false);
}

void AuthWidget::onDataReady()
{
}

void AuthWidget::onLoadeFinished(bool ok)
{
    if (ok) {
        mView->setVisible(true);
        update();
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
    qDebug() << Q_FUNC_INFO << progress;
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
        configState(DesktopWidget::DOCK);
        this->setVisible(false);
        /* save the auth token */
        PlexyDesk::Config *config = PlexyDesk::Config::getInstance();
        config->beginGroup("facebook_plugin");
        config->setValue("access_token",
                fburl.queryItemValue("access_token"));
        config->endGroup();
    }
}
void AuthWidget::readConfig(QString& user,
        QString& pass)
{
}

void AuthWidget::data(QVariantMap& data)
{
    qDebug() << Q_FUNC_INFO << data["data"];
}

void AuthWidget::onReadyRead()
{
    JsonData result = mJsonHandle->parse(mReply->readAll());
    if (result.type() == JsonData::Error) {
        this->setVisible(true);
        configState(DesktopWidget::NORMALSIDE);
        mView->setVisible(false);
        mView->setUrl(QUrl(QLatin1String("https://graph.facebook.com/oauth/authorize?client_" \
                    "id=170356722999159&redirect_uri=http://www.facebook.com/connect" \
                    "/login_success.html&type=user_agent&display=popup")));

    }
}

QString AuthWidget::tokenFromConfig() const
{
    PlexyDesk::Config *config = PlexyDesk::Config::getInstance();
    config->beginGroup("facebook_plugin");
    QString token = config->value("access_token").toString();
    config->endGroup();
    qDebug() << Q_FUNC_INFO << token;
    return token;
}
}
