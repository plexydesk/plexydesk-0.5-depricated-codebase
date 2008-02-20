#include "desktopview.h"
#include <backdropfactory.h>
#include <abstractplugininterface.h>
#include <QGLWidget>


namespace PlexyDesk
{
class  DesktopView::Private
{
    public:
    Private(){}
    ~Private(){}
    ExtensionProducer<BackdropInterface> fact;
    BackdropInterface * bIface ;
    
    BackdropFactory * bgfact;
};

DesktopView::DesktopView ( QGraphicsScene * scene, QWidget * parent ):QGraphicsView(scene,parent),d(new Private)
{
       setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
      /// setCacheMode(QGraphicsView::CacheBackground);
       setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

      // setRenderHints(QPainter::HighQualityAntialiasing);
      // setOptimizationFlag(QGraphicsView::DontClipPainter,false);
      // setOptimizationFlag(QGraphicsView::DontSavePainterState,true);
       setAlignment(Qt::AlignLeft | Qt::AlignTop);
       d->bIface  = 0;
     
//       PluginLoader loader;
       QString defaults = PluginLoader::getInstance()->listPlugins("Desktop").first();
       d->bIface = (BackdropInterface*)PluginLoader::getInstance()->instance(defaults);
       
       // qDebug()<<d->bIface<<endl;
    //   setInteractive(true);



}

DesktopView::~DesktopView()
{
    delete d;
}


void DesktopView::drawBackground ( QPainter * painter, const QRectF & rect )
{
    painter->save();
    painter->setClipRect(rect);
    if(d->bIface){
  	d->bIface->render(painter,QRectF(rect.x(),rect.y(),rect.width(),rect.height()));
    }
    painter->restore();
}


}
#include "desktopview.moc"
