#include <canvas.h>


namespace PlexyDesk
{

Canvas::Canvas (QObject * parent): QGraphicsScene(parent)
{
} 

Canvas::~Canvas()
{
  
}  

void Canvas::drawBackground ( QPainter * painter, const QRectF & rect )
{
 // painter->setOpacity(0.0);
 // painter->setCompositionMode(QPainter::CompositionMode_Source);
//  painter->fillRect(rect,QColor(Qt::transparent));
}

} /* PlexyDesk */

#include "canvas.moc"