/* This file is part of telepathy-accountmanager-kwallet
 *
 * Copyright (C) 2008-2009 Collabora Ltd. <http://www.collabora.co.uk/>
 * Mahesh Kaushalya 2008-2009 <wpmahesh@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 *Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "storagelayerinterface.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace PlexyDesk
{
class SocialStorageLayer : public StorageLayerInterface
{
    Q_OBJECT;
public:
    SocialStorageLayer(QObject *parent=0);
    void open();
    void close() ;

    QStringList listAccounts() ;
    QVariantMap readAccount(const QString &id);
    bool removeAccount(const QString &id);
    bool writeAccount(const QString &id, const QVariantMap &data);
    bool updateAccount(const QString &id, const QString &propertyName, const QVariant &value);

private:
    void setDatabasePath(const QString &path);
    void createTables();

    //QString generatePasswordHash();
};
}
