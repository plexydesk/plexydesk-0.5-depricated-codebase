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

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>

namespace PlexyDesk
{
class StorageLayerInterface : public QObject
{
    Q_OBJECT;

public:
    StorageLayerInterface(QObject *parent = 0): QObject(parent) {}
    virtual ~StorageLayerInterface() {}

    virtual void open() = 0;
    virtual void close() = 0;

    virtual QStringList listAccounts() = 0;
    virtual QVariantMap readAccount(const QString& id) = 0;
    virtual bool removeAccount(const QString& id) = 0;
    virtual bool writeAccount(const QString& id, const QVariantMap& data) = 0;
    virtual bool updateAccount(const QString &id, const QString &propertyName, const QVariant &value) = 0;

signals:
    void opened(bool success);
    void closed();
};
}
