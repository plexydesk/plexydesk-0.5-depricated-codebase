
#ifndef PLEXY_DESK_BACK_DROP_INTERFACE
#define PLEXY_DESK_BACK_DROP_INTERFACE

#include <QtCore>
#include <QtGui>
#include <abstractplugininterface.h>

namespace PlexyDesk
{
    class BackdropInterface : public AbstractPluginInterface 
    {
        public:
            void init(){}
            BackdropInterface(){}
            ~BackdropInterface(){}
            virtual void render(QPainter *p,QRectF r) = 0;
    };
} // namespace PlexyDesk

Q_DECLARE_INTERFACE(PlexyDesk::BackdropInterface, "org.plexydesk.BackdropInterface")
#endif
