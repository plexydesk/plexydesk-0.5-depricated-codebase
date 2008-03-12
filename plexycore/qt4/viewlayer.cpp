#include  "viewlayer.h"


namespace PlexyDesk
{

ViewLayer::ViewLayer(QObject * obj) : QObject(obj)
{
     currentList  
}

ViewLayer::~ViewLayer ()
{

}

void ViewLayer::addItem ( const QString& layerName, DesktopWidget * item)
{
    if ( this->layer.contains(layerName) ) {
        this->layer[layerName]->append(item);
    } else {
        List* itemlayer   = new List;
        itemlayer->append (item);
        this->layer[layerName] = itemlayer;
    }
}


void ViewLayer::showLayer (const QString& layername)
{
    
}

} // namespace PlexyDesk






#include "viewlayer.moc"
