#ifndef ABSTRACT_DATA_SOURCE_H
#define ABSTRACT_DATA_SOURCE_H

#include <plexy.h>

#include <QCoreApplication>
#include <QGraphicsItem>
#include <QObject>
#include <QString>
#include <QSettings>
#include <QDir>


namespace PlexyDesk
{
class PLEXYDESK_EXPORT AbstractSource : public QObject
{
    Q_OBJECT

public:
    AbstractSource(QObject *parent = 0) : QObject(parent)
    {
    }

};
} // namespace PlexyDesk

#endif
