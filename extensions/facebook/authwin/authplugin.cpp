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
#include "facebookcontactcard.h"
#include <qwebviewitem.h>
#include "facebookauthenticationwidget.h"
#include <abstractdesktopview.h>

AuthPlugin::AuthPlugin(QObject *object) : PlexyDesk::ControllerInterface (object)
{
     mWidget = new FacebookAuthenticationWidget(QRectF(0, 0, 480, 320));
     mWidget->setController(this);
     mWidget->setVisible(true);
     mWidget->setLabelName("Web Auth");

     connect(mWidget, SIGNAL(facebookToken(QString)), this, SLOT(onFacebookToken(QString)));


     mContactUI = new FacebookContactUI(QRectF(0.0, 0.0, 488.0, 320.0));
     mContactUI->setController(this);
     mContactUI->setVisible(true);
     mContactUI->setLabelName("Contacts");

     connect(mContactUI, SIGNAL(addContactCard(QString)), this, SLOT(onAddContactCard(QString)));

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

void AuthPlugin::requestFriendsList(QString token, const QVariantMap &args)
{
    PlexyDesk::DataSource *fbSource = dataSource();
    QVariantMap request;
    QVariant arg;
    request["command"] = QVariant("friends");
    request["token"] = token;
    mToken = token;
    arg = request;

    if (fbSource)
        fbSource->setArguments(arg);
}

void AuthPlugin::revokeSession(const QVariantMap &args)
{
    QString token = args["access_token"].toString();

    if (token.isNull() || token.isEmpty()) {
        requestFacebookSession();
        return;
    }
    mWidget->setVisible(false);

    requestFriendsList(token, args);
    if (!args["contact"].toString().isEmpty()) {

        mContacts = args["contact"].toString().split(',');

        if (mContacts.count() <= 0) {
            onAddContactCard(args["contact"].toString());
        }

        Q_FOREACH (const QString &id, args["contact"].toString().split(',')) {
            onAddContactCard (id);
        }
    }
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

void AuthPlugin::handleViewport()
{
    requestFacebookSession();
}

QStringList AuthPlugin::actions() const
{
    QStringList rv;
    rv << "Friends Browser" << "Feed Wall";

    return rv;
}

void AuthPlugin::requestAction(const QString &actionName, const QVariantMap &args)
{
    if (actionName == "Friends Browser") {
        if (mContactUI) {
            mContactUI->show();
        } else {
            mContactUI = new FacebookContactUI(QRectF(0.0, 0.0, 488.0, 320.0));
            mContactUI->setController(this);
            mContactUI->setVisible(true);

            connect(mContactUI, SIGNAL(addContactCard(QString)), this, SLOT(onAddContactCard(QString)));

            if (viewport()) {
                viewport()->addWidgetToView(mContactUI);
                requestFriendsList(mToken, args);
            }
        }
    }
}

bool AuthPlugin::disconnectFromDataSource(PlexyDesk::AbstractDesktopWidget *widget)
{
    if (widget == mContactUI) {
        delete widget;
        mContactUI = 0;
        return true;
    }

    return false;
}


void AuthPlugin::onDataUpdated(const QVariantMap &map)
{
    QString command = map["command"].toString();

    if (command == "login") {
        QString key = map["token"].toString();

        if (key.isEmpty() || key.isNull()) {
            //request login UI
            if (mWidget) {
                if (viewport())
                    viewport()->addWidgetToView(mWidget);
            }
        }
    }

    if (command == "friends") {
        if (mContactUI)
            mContactUI->setFacebookContactData(map["data"].toHash());
    }

    if (command == "userinfo") {
        if (mContactUI)
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
    mToken = token;

    if (fbSource)
        fbSource->setArguments(arg);
}

void AuthPlugin::onAddContactCard(const QString &id)
{
    FacebookContactCard *contactCard = new FacebookContactCard(QRectF(0.0, 0.0, 320, 480), 0);
    contactCard->setPos(50, 50);
    contactCard->setDataSource(id, mToken, dataSource());
    contactCard->setController(this);
    contactCard->setLabelName(id);

    if (!mContacts.contains(id)) {
        mContacts.append(id);
    }

    if (viewport())
        viewport()->addWidgetToView(contactCard);

    if (viewport()) {
        viewport()->sessionDataForController(controllerName(),"contact", mContacts.join(","));
    }
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
