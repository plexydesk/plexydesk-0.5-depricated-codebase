
#ifndef PLEXY_DATA_INTERFACE
#define PLEXY_DATA_INTERFACE

#include <QtCore>
#include <QtGui>
#include <abstractplugininterface.h>

namespace PlexyDesk
{
    class DataInterface : public AbstractPluginInterface 
    {
        public:
	void init() {}
            DataInterface(){}
            ~DataInterface(){}
           virtual  QGraphicsItem * item() = 0;
        public slots:
        signals:
           virtual void data(QVariant&)=0;
    };
} // namespace PlexyDesk

Q_DECLARE_INTERFACE(PlexyDesk::DataInterface, "org.plexydesk.DataInterface")
#endif
