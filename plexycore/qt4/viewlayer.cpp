#include  "viewlayer.h"


namespace PlexyDesk
{

ViewLayer::ViewLayer(QObject * obj) : QObject(obj)
{
        currentList = new List;
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
        if (!layer.contains(layername)) {
                qDebug("Invalid Layer:  ViewLayer::showLayer()");
        } else {
                for (int i = 0; i < currentList->size();i++) {
                        if ( currentList->at(i) ) {
                                currentList->at(i)->hide();
                        }
                }

                currentList  = this->layer[layername];

                for (int i = 0; i < currentList->size();i++) {
                        if ( currentList->at(i) ) {
                                currentList->at(i)->show();
                        }
                }
        }
}

} // namespace PlexyDesk






#include "viewlayer.moc"
