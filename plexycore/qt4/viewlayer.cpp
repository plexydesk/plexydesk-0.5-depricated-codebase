#include  "viewlayer.h"


namespace PlexyDesk
{

ViewLayer::ViewLayer(QObject * obj) : QObject(obj)
{
     
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

} // namespace PlexyDesk






#include "viewlayer.moc"
