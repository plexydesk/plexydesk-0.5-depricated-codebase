#ifndef BASE_PLUGIN_H
#define BASE_PLUGIN_H

#include <QtCore>

namespace PlexyDesk
{
    class BasePlugin : public QObject
    {
        Q_OBJECT
        public:
            BasePlugin (QObject *parent = 0);
        
    };
} // namespace PlexyDesk

#endif
