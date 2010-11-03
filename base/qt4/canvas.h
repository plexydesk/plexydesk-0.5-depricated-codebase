#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <QGraphicsScene>
#include <plexyconfig.h>
#include <backdropinterface.h>
#include <pluginloader.h>
#include <baseplugin.h>
#include <backdropplugin.h>
#include <widgetplugin.h>

namespace PlexyDesk
{
    class PLEXYDESK_EXPORT Canvas : public QGraphicsScene
    {
        Q_OBJECT

    public:
        Canvas(QObject *parent = 0);
        virtual ~Canvas();

    protected:
        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    private:
        class Private;
        Private *const d;
    };
} /* PlexyDesk */


#endif /* _CANVAS_H_ */
