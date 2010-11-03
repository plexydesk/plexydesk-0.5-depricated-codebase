#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <QObject>
#include <QString>
#include <QCoreApplication>
#include <plexy.h>

namespace PlexyDesk
{
    class PLEXYDESK_EXPORT BasePlugin : public QObject
    {
        Q_OBJECT

    public:
        BasePlugin(QObject *parent = 0): QObject(parent)
        {
        }

        QString applicationDirPath() const
        {
            return QString(PLEXPREFIX);
        }
    };
} // namespace PlexyDesk

#endif
