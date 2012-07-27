#ifndef ABSTRACT_DATA_SOURCE_H
#define ABSTRACT_DATA_SOURCE_H

#include <plexy.h>
#include <QtCore>

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
