/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include <canvas.h>


namespace PlexyDesk
{

class Canvas::Private
{
public:
    Private() {}
    ~Private() {}
    AbstractPluginInterface * bIface ;
    BackdropPlugin * bgPlugin;
};

Canvas::Canvas (QObject * parent): QGraphicsScene(parent), d(new Private)
{
  //  d->bgPlugin  = static_cast<BackdropPlugin*>(PluginLoader::getInstance()->instance("classicbackdrop"));
} 

Canvas::~Canvas()
{
  
}  

void Canvas::drawBackground ( QPainter * painter, const QRectF & rect )
{
  /*
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->fillRect(rect, Qt::transparent);
    painter->save();
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setClipRect(rect);
    if (d->bgPlugin) {
        d->bgPlugin->render(painter,QRectF(rect.x(),rect.y(),rect.width(),rect.height()));
    }
    painter->restore();
    */
}

} /* PlexyDesk */

#include "canvas.moc"