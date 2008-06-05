#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <QtCore>
#include <config.h>

namespace PlexyDesk
{
class BasePlugin : public QObject
{
    Q_OBJECT
public:
    BasePlugin(QObject *parent = 0): QObject(parent) {}
    QString applicationDirPath()
    {
#ifdef Q_WS_WIN
        return QString(QCoreApplication::applicationDirPath() + "/..");
#else
        return QString(PLEXPREFIX);
#endif
    }

};
} // namespace PlexyDesk

#endif
