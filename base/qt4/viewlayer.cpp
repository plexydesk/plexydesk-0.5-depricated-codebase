#include  "viewlayer.h"

namespace PlexyDesk
{
class ViewLayer::Private
{
public:
    Private() {}
    ~Private() {}

    typedef QList <DesktopWidget*> List;
    typedef QHash <QString, List*> Layer;

    Layer layer;
    List * currentList;
};

ViewLayer::ViewLayer(QObject * obj) : QObject(obj),  d(new Private)
{
    d->currentList = new Private::List;
}

ViewLayer::~ViewLayer()
{
    delete d;
}

void ViewLayer::addItem(const QString& layerName, DesktopWidget * item)
{
    if (d->layer.contains(layerName)) {
        d->layer[layerName]->append(item);
    } else {
        Private::List* itemlayer   = new Private::List;
        itemlayer->append(item);
        d->layer[layerName] = itemlayer;
    }
}

void ViewLayer::showLayer(const QString& layername)
{
    if (!d->layer.contains(layername)) {
        qDebug("Invalid Layer:  ViewLayer::showLayer()");
    } else {
        for (int i = 0; i < d->currentList->size(); i++) {
            if (d->currentList->at(i)) {
                d->currentList->at(i)->hide();
            }
        }

        d->currentList  = d->layer[layername];

        for (int i = 0; i < d->currentList->size(); i++) {
            if (d->currentList->at(i)) {
                d->currentList->at(i)->show();
            }
        }
    }
}

QStringList ViewLayer::layerIndex() const
{
   return d->layer.keys();
}

} // namespace PlexyDesk
