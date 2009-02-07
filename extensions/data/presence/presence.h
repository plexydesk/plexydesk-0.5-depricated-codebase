/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Dariusz Mikulski <dariusz.mikulski@gmail.com>
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

#ifndef PRESENCE_H_
#define PRESENCE_H_

#include <datainterface.h>
#include <TelepathyQt4/Client/PendingOperation>

class QDBusObjectPath;

class VISIBLE_SYM PresenceData : public PlexyDesk::DataPlugin
{
    Q_OBJECT

public:
    PresenceData(QObject * object = 0);
    virtual ~PresenceData();

    void init();
    void pushData(QVariant&);

signals:
    void data(QVariant&);

private slots:
	void onAccountReady(Telepathy::Client::PendingOperation *operation);
    void accountCreated(const QString &path);
    void accountRemoved(const QString &path);
    void accountValidityChanged(const QString &path, bool valid);

private:
	class PresenceDataPrivate;
	PresenceDataPrivate * const d;
};

#endif //PRESENCE_H_