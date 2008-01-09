
#ifndef PLEXY_BACK_DROP_FACTORY_H
#define PLEXY_BACK_DROP_FACTORY_H

#include <extensionfactory.h>
#include <QtCore>
#include <QtGui>
#include <plexy.h>

namespace PlexyDesk
{
class VISIBLE_SYM BackdropFactory : public QObject, public ExtensionFactory 
{

    Q_OBJECT 
    public:
        typedef QHash<QString,BackdropInterface*> InterfaceDict;
        BackdropFactory(QObject * object = 0);
        virtual ~BackdropFactory();

       virtual void * instance()
        {
           ExtensionProducer<BackdropInterface> obj;
          return obj.createObject();
        }
    private:
    //   class Private;
     //  Private * const d;
    
        class Private;
        Private * const d ;

};

}
#endif
