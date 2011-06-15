#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <plexy.h>

#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QSettings>
#include <QDir>


namespace PlexyDesk
{
class PLEXYDESK_EXPORT BasePlugin : public QObject
{
    Q_OBJECT

public:
    BasePlugin(QObject *parent = 0) : QObject(parent)
    {
    }

    QString applicationDirPath() const
    {
        return QDir::toNativeSeparators(PLEXPREFIX);
    }

    QHash<QString, QString> metaData() const
    {
        return QHash<QString, QString>();
    }
};
} // namespace PlexyDesk

#endif
