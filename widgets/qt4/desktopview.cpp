#include "desktopview.h"
#include <backdropfactory.h>


namespace PlexyDesk
{
class  DesktopView::Private
{
    public:
    Private(){}
    ~Private(){}
    BackdropInterface * bIface;
    BackdropFactory * bgfact;
};

DesktopView::DesktopView ( QGraphicsScene * scene, QWidget * parent ):QGraphicsView(scene,parent),d(new Private)
{
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setCacheMode(QGraphicsView::CacheBackground);
       setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
       setRenderHint(QPainter::Antialiasing);
       //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
     //  centerOn(0,0);
     //
       setOptimizationFlag(QGraphicsView::DontClipPainter,true);
       setOptimizationFlag(QGraphicsView::DontSavePainterState,true);
       setAlignment(Qt::AlignLeft | Qt::AlignTop);
       d->bIface  = 0;
       d->bgfact = new BackdropFactory(0);
       d->bIface = d->bgfact->instance();
       setInteractive(true);
}

DesktopView::~DesktopView()
{
    delete d;
}

void DesktopView::drawBackground ( QPainter * painter, const QRectF & rect )
{
    d->bIface->render(painter);
}

}
#include "desktopview.moc"
