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
#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <abstractplugininterface.h>
#include <plexy.h>

#include <QHash>
#include <QDir>

namespace PlexyDesk
{
class PLEXYDESK_EXPORT PluginLoader : public QObject
{
    Q_OBJECT

public:
    typedef QHash <QString, AbstractPluginInterface *> Interface;

    PluginLoader();
    virtual ~PluginLoader();

    static PluginLoader *getInstance();

    QStringList listPlugins(const QString &types);
    AbstractSource *instance(const QString &name);
    void scanDisk();
    void setPluginPrefix(const QString &path);
    QString pluginPrefix() const;

protected:
    void loadDesktop(const QString &path);
    void load(const QString &_interface, const QString &plugin);

private:
    class Private;
    Private *const d;
#ifdef Q_WS_WIN
    static PluginLoader *mInstance;
#else
    static PLEXYDESK_EXPORT PluginLoader *mInstance;
#endif
};

} // namespace PlexDesk
#endif
