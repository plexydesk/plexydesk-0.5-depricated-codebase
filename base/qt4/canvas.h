#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <QGraphicsScene>
#include <plexyconfig.h>

/**
 * \class Canvas
 * \brief
 */
namespace PlexyDesk
{
class VISIBLE_SYM Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    Canvas(QObject * parent = 0);
    virtual ~Canvas();
protected:
    virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
private:
    class Private;
    Private * const d;
};
} /* PlexyDesk */


#endif /* _CANVAS_H_ */
