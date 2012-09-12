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
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    d->mBackgroundItem = 0;
    d->mBackgroundSource = 0;
    setBackgroundSource("classicbackdrop");
}

bool PlexyDesktopView::setBackgroundSource(const QString &source_id)
{

    //TODO: error handling
    // delete the current background source before setting a new one
    if (d->mBackgroundSource)
        return 0;

    d->mBackgroundSource =
            qobject_cast<PlexyDesk::BackgroundSource*> (PlexyDesk::PluginLoader::getInstance()->instance(source_id));

    if (!d->mBackgroundSource)
        return 0;

    QRectF screenRect = scene()->sceneRect();
    qDebug() << Q_FUNC_INFO << screenRect;
    d->mBackgroundSource->setRect (
                QRect (screenRect.x(),
                       screenRect.y(),
                       screenRect.width(),
                       screenRect.height()));

    d->mBackgroundItem = d->mBackgroundSource->item();

    scene()->addItem(d->mBackgroundItem);

    d->mBackgroundItem->show();

    return true;
}
