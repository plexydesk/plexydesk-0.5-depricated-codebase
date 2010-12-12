/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>
#include <QXmlQuery>
#include <QXmlSerializer>

#include "authwidget.h"

namespace PlexyDesk{

AuthWidget::AuthWidget(const QRectF &rect, QWidget *widget):
        PlexyDesk::DesktopWidget(rect, widget)
{
    QRect webrect = QRect(10.0, 10.0, rect.width()-65, rect.height()-5);
    mView = new QWebViewItem(webrect, this);
    mView->setUrl(QUrl("https://graph.facebook.com/oauth/authorize?client_id=170356722999159&redirect_uri=http://www.facebook.com/connect/login_success.html&type=user_agent&display=popup"));
    mView->setOpacity(0.3);
}

AuthWidget::~AuthWidget()
{
}

void AuthWidget::paintExtFace(QPainter *painter,
        const QStyleOptionGraphicsItem *, QWidget *)
{
    mView->setVisible(true);
}
void AuthWidget::paintExtDockFace(QPainter *painter,
        const QStyleOptionGraphicsItem *, QWidget *)
{
    mView->setVisible(false);
}

void AuthWidget::onDataReady()
{
}

void AuthWidget::readConfig(QString& user,
        QString& pass)
{
}

void AuthWidget::data(QVariantMap& data)
{
    qDebug() << Q_FUNC_INFO << data["data"];
}
}
