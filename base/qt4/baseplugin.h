#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <plexy.h>

#include <QCoreApplication>
#include <QObject>
#include <QString>


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
