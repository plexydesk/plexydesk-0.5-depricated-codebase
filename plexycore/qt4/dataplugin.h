
#ifndef PLEXY_DATA_PLUGIN_H
#define PLEXY_DATA_PLUGIN_H 

#include <plexy.h>
#include <config.h>

#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
    class DataPlugin : public QObject
    {
        Q_OBJECT
        public :
                DataPlugin(QObject  * object = 0){}
                virtual ~DataPlugin(){}
        public slots:
                virtual void pushData(QVariant&) = 0;
        signals:
                virtual void data(QVariant&)=0;

     };
}
#endif
