
#ifndef PLEXY_BACK_DROP_FACTORY_H
#define PLEXY_BACK_DROP_FACTORY_H

#include <extensionfactory.h>
#include <QtCore>
#include <QtGui>
#include <plexy.h>

namespace PlexyDesk
{
class VISIBLE_SYM BackdropFactory : public QObject
{

    Q_OBJECT 
    public:
        typedef QHash<QString,BackdropInterface*> InterfaceDict;
        BackdropFactory(QObject * object = 0);
        virtual ~BackdropFactory();

       virtual BackdropInterface * instance();
     private:
    //   class Private;
     //  Private * const d;
    
        class Private;
        Private * const d ;

};

}
#endif
