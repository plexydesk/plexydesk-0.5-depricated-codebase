#include "listview.h"

namespace PlexyDesk
{
    class ListView::Private
    {
        public:
        Private() {}
        ~Private(){}
    };

ListView::ListView(const QRectF &rect, QWidget *win):
        DesktopWidget(rect, win), d(new Private)
{
}

ListView::~ListView()
{
    delete d;
}

void ListView::paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem * e, QWidget *)
{
}

void ListView::wheelEvent(QGraphicsSceneWheelEvent * event)
{
}

void ListView::paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void ListView::drawWidget()
{
}

}
