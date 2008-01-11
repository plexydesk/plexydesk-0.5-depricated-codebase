#include "desktopview.h"
#include <backdropfactory.h>
#include <QGLWidget>


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
//        setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
       setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setCacheMode(QGraphicsView::CacheBackground);
       setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
       setRenderHint(QPainter::Antialiasing);
       setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
     //  centerOn(0,0);
     //
       setOptimizationFlag(QGraphicsView::DontClipPainter,true);
       setOptimizationFlag(QGraphicsView::DontSavePainterState,false);
       setAlignment(Qt::AlignLeft | Qt::AlignTop);
       d->bIface  = 0;
       d->bgfact = new BackdropFactory(0);
       d->bIface = d->bgfact->instance();
   //    fitInView(QDesktopWidget().screenGeometry());
       setInteractive(true);
     //  scale(2,2);
}

DesktopView::~DesktopView()
{
    delete d;
}

void DesktopView::drawBackground ( QPainter * painter, const QRectF & rect )
{
    //qDebug()<<rect<<endl;
    painter->save();
    painter->setBrushOrigin(0, 0);
    d->bIface->render(painter,QRectF(rect.x(),rect.y(),rect.width(),rect.height()));
    painter->restore();
}

}
#include "desktopview.moc"
