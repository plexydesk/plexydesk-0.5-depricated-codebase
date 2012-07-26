#include <QDebug>
#include  "viewlayer.h"

namespace PlexyDesk
{
class ViewLayer::Private
{
public:
    Private() {}
    ~Private()
    {
        clean();
    }

    typedef QList <AbstractDesktopWidget *> List;
    typedef QHash <QString, List *> Layer;

    Layer layer;
    List *currentList;
    QString currentLayerName;

private:
    void clean();
};

void ViewLayer::Private::clean()
{
    qDebug() << "Cleaning layers";

    QStringList keysList = layer.keys();
    foreach(QString key, keysList)
    {
        List *widgetList = layer.value(key);
        widgetList->clear();
        delete widgetList;
        layer.remove(key);
    }

    layer.clear();
}

ViewLayer::ViewLayer(QObject *obj) : QObject(obj), d(new Private)
{
    d->currentList = new Private::List;
}

ViewLayer::~ViewLayer()
{
    delete d;
}

void ViewLayer::addItem(const QString &layerName, AbstractDesktopWidget *item)
{
    if (d->currentLayerName != layerName) {
        item->hide();
    }

    if (d->layer.contains(layerName)) {
        d->layer[layerName]->append(item);
    } else {
        Private::List *itemlayer = new Private::List;
        itemlayer->append(item);
        d->layer[layerName] = itemlayer;
    }
}

void ViewLayer::showLayer(const QString &layername)
{
    /* Hide all the stuff in the current layer
       then show the requested layer
     */

    if (!d->layer.contains(layername)) {
        qDebug() << Q_FUNC_INFO << "Invalid Layer";
    } else {
        qDebug() << Q_FUNC_INFO << layername;
        hideLayer();
    d->currentList = d->layer[layername];
        for (int i = 0; i < d->currentList->size(); i++) {
            if (d->currentList->at(i)) {
                d->currentList->at(i)->show();
            }
        }

        d->currentLayerName = layername;
    }
}
void ViewLayer::hideLayer()
{
     for (int i = 0; i < d->currentList->size(); i++) {
        if (d->currentList->at(i)) {
            d->currentList->at(i)->hide();
        }
     }
}

void ViewLayer::switchLayer()
{
    QString newLayer;
    QString currentLayer = d->layer.key(d->currentList);
    QList<QString> keysList = d->layer.keys();
    int totalLayers = d->layer.count();
    int index = -1;
    for (int i = 0; i<totalLayers; i++)
    {
        if (keysList[i]==currentLayer)
            index = i;
    }
    if (index != totalLayers - 1)
    {
        newLayer = keysList.at(index+1);
    }
    else
    {
        newLayer = keysList.at(0);
    }
    showLayer(newLayer);

    qDebug()<< "CurrentLayer::" <<currentLayer;
    qDebug()<< "NewLayer::" <<newLayer;
}
QStringList ViewLayer::layerIndex() const
{
    return d->layer.keys();
}

} // namespace PlexyDesk
