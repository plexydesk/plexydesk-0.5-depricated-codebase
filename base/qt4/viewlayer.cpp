#include  "viewlayer.h"

namespace PlexyDesk
{
class ViewLayer::Private
{
public:
    Private() {
    }
    ~Private() {
    }

    typedef QList <DesktopWidget *> List;
    typedef QHash <QString, List *> Layer;

    Layer layer;
    List *currentList;
};

ViewLayer::ViewLayer(QObject *obj) : QObject(obj), d(new Private), p(new Private)
{
    d->currentList = new Private::List;
}

ViewLayer::~ViewLayer()
{
    delete d;
    delete p;
}

void ViewLayer::addItem(const QString &layerName, DesktopWidget *item)
{
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
    if (!d->layer.contains(layername)) {
        qDebug("Invalid Layer:  ViewLayer::showLayer()");
    } else {
        qDebug()<<Q_FUNC_INFO <<layername;
        for (int i = 0; i < d->currentList->size(); i++) {
            if (d->currentList->at(i)) {
                d->currentList->at(i)->hide();
            }
        }
        d->currentList = d->layer[layername];
        for (int i = 0; i < d->currentList->size(); i++) {
            if (d->currentList->at(i)) {
                d->currentList->at(i)->show();
            }
        }
    }
}
void ViewLayer::hideLayer(const QString &layerName)
{
    if (!d->layer.contains(layerName)) {
        qDebug("Invalid Layer:  ViewLayer::hideLayer()");
    } else {
        qDebug()<<Q_FUNC_INFO <<layerName;
        p->currentList = d->currentList;
        d->currentList = d->layer[layerName];
        p->currentList = d->currentList;
        d->currentList = d->layer[layerName];
        for (int i = 0; i < d->currentList->size(); i++) {
            if (d->currentList->at(i)) {
                d->currentList->at(i)->hide();
            }
        }
        d->currentList = p->currentList;
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
    hideLayer(currentLayer);
    showLayer(newLayer);

    qDebug()<<"CurrentLayer::"<<currentLayer;
    qDebug()<<"NewLayer::"<<newLayer;
}
QStringList ViewLayer::layerIndex() const
{
    return d->layer.keys();
}

} // namespace PlexyDesk
