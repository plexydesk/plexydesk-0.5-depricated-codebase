#include "plexydesktopview.h"

#include <datasource.h>
#include <pluginloader.h>
#include <backdropplugin.h>
#include <QGraphicsItem>


class PlexyDesktopView::PrivatePlexyDesktopView
{
public:
    PrivatePlexyDesktopView() {}
    ~PrivatePlexyDesktopView() {}

     PlexyDesk::BackgroundSource *mBackgroundSource;
     QGraphicsItem *mBackgroundItem;
};

PlexyDesktopView::PlexyDesktopView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent),
    d(new PrivatePlexyDesktopView)
{
    d->mBackgroundItem = 0;
    d->mBackgroundSource = 0;
    setBackgroundSource("classicbackdrop");
}

bool PlexyDesktopView::setBackgroundSource(const QString &source_id)
{

    d->mBackgroundSource =
            qobject_cast<PlexyDesk::BackgroundSource*> (PlexyDesk::PluginLoader::getInstance()->instance(source_id));

    if (!d->mBackgroundSource)
        return 0;


    d->mBackgroundItem = d->mBackgroundSource->item();

    scene()->addItem(d->mBackgroundItem);

    return true;
}
