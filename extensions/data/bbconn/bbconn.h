/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
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
#ifndef TIMER_DATA_H
#define TIMER_DATA_H

#include <QtCore>
#include <plexy.h>
#include <abstractplugininterface.h>
#include <datasource.h>
#include <QtNetwork>

#include "connection.h"

class BBConnData : public PlexyDesk::DataSource
{
    Q_OBJECT

public:
    BBConnData(QObject *object = 0);
    virtual ~BBConnData();
    void init();
    QVariantMap readAll();

    void timerEvent (QTimerEvent *event);

    void startService(const QString &token);
    QString encrypt(const QString &token) const;

public Q_SLOTS:
    void setArguments(QVariant sourceUpdated);
    void onNewMessage(const QString &from, const QString &message);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void onApprovalRequested(const QString &token, Connection *conn);
    void loadKeys();

    // Slots to receive signals from sockets
    void acceptConnection();
    void handshakeComplete();
    void sslErrors(const QList<QSslError> &errors);
    void receiveMessage();
    void connectionClosed();
    void connectionFailure();

private:
    class Private;
    Private *const d;
};

#endif
