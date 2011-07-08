#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <plexy.h>

#include <QCoreApplication>
#include <QGraphicsItem>
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
};
} // namespace PlexyDesk

#endif
