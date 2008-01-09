
#ifndef PLEXY_WIDGET_FACTORY_H
#define PLEXY_WIDGET_FACTORY_H

#include <extensionfactory.h>
#include <QtCore>
#include <QtGui>
#include <plexy.h>

namespace PlexyDesk
{
class VISIBLE_SYM WidgetFactory : public QObject, public WidgetExtensionFactory 
{

    Q_OBJECT 
    public:
        typedef QHash<QString,BackdropInterface*> InterfaceDict;
        WidgetFactory(QObject * object = 0);
        virtual ~WidgetFactory();

       virtual WidgetInterface * instance();
     private:
        class Private;
        Private * const d ;

};

}
#endif
