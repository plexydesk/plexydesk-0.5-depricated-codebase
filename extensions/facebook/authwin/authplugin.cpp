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

#include "authplugin.h"
#include <qwebviewitem.h>

AuthPlugin::AuthPlugin(QObject *object) : PlexyDesk::ControllerInterface (object)
{
     mWidget = new PlexyDesk::AuthWidget(QRectF(0, 0, 480, 320));
     mWidget->setController(this);
     mWidget->setVisible(false);

     connect(mWidget, SIGNAL(facebookToken(QString)), this, SLOT(onFacebookToken(QString)));

     mContactUI = new FacebookContactUI(QRectF(0.0, 0.0, 488.0, 320.0));
     mContactUI->setController(this);
     mContactUI->setVisible(true);

     if (connectToDataSource("facebookengine")) {
         connect(dataSource(), SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onDataUpdated(QVariantMap)));
     }
}

AuthPlugin::~AuthPlugin()
{
    if (mWidget)
        delete mWidget;
}

PlexyDesk::AbstractDesktopWidget *AuthPlugin::defaultView()
{
    return mContactUI;
}

void AuthPlugin::revokeSession(const QVariantMap &args)
{
    QString token = args["access_token"].toString();

    if (token.isNull() || token.isEmpty()) {
        requestFacebookSession();
        return;
    }
    mWidget->setVisible(false);

    PlexyDesk::DataSource *fbSource = dataSource();
    QVariantMap request;
    QVariant arg;
    request["command"] = QVariant("friends");
    request["token"] = args["access_token"];
    arg = request;

    if (fbSource)
        fbSource->setArguments(arg);
}

void AuthPlugin::setViewRect(const QRectF &rect)
{
    if (mWidget)
        mWidget->setPos(rect.x(), rect.y());

    if (mContactUI)
        mContactUI->setPos(rect.x(), rect.y());
}

void AuthPlugin::firstRun()
{
    requestFacebookSession();
}

void AuthPlugin::onDataUpdated(const QVariantMap &map)
{
    QString command = map["command"].toString();

    if (command == "login") {
        QString key = map["token"].toString();

        if (key.isEmpty() || key.isNull()) {
            //request login UI
            if (mWidget) {
                Q_EMIT spawnView(mWidget);
                mWidget->setVisible(true);
                mWidget->createAuthDialog();
            }
        }
    }

    if (command == "friends") {
        qDebug() << map.keys();
        mContactUI->setFacebookContactData(map["data"].toHash());
    }

    if (command == "userinfo") {
        mContactUI->addContact(map);
    }
}

void AuthPlugin::onFacebookToken(const QString &token)
{
    PlexyDesk::DataSource *fbSource = dataSource();
    QVariantMap request;
    QVariant arg;
    request["command"] = QVariant("friends");
    request["token"] = token;
    arg = request;

    if (fbSource)
        fbSource->setArguments(arg);
}

void AuthPlugin::requestFacebookSession()
{
    PlexyDesk::DataSource *fbSource = dataSource();

    QVariantMap request;
    QVariant arg;
    request["command"] = QVariant("login");
    arg = request;

    if (fbSource)
        fbSource->setArguments(arg);
}
