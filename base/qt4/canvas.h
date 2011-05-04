#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <backdropinterface.h>
#include <backdropplugin.h>
#include <baseplugin.h>
#include <plexyconfig.h>
#include <pluginloader.h>
#include <widgetplugin.h>

#include <QGraphicsScene>

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
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

private:
    class Private;
    Private *const d;
};
} /* PlexyDesk */


#endif /* _CANVAS_H_ */
