
#ifndef VIEW_LAYER_H
#define VIEW_LAYER_H


#include <QtCore>
#include <QtGui>
#include <config.h>

#include <plexy.h>
#include <desktopwidget.h>

namespace PlexyDesk
{
    class ViewLayer : public  QObject 
    {
        Q_OBJECT
        public:
            ViewLayer ( QObject* object = 0);
            virtual ~ViewLayer ();
        private:
            typedef QList <DesktopWidget*> List;
            typedef QMap <QString,List*> Layer;

            List list;
            Layer layer;

    };
} // namespace PlexyDesk

#endif
