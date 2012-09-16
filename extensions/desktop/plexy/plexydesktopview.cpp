#include "plexydesktopview.h"

#include <datasource.h>
#include <pluginloader.h>
#include <backdropplugin.h>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "iconwidgetview.h"
#include "fileiconwidget.h"
#include "plexygraphicsscene.h"


class PlexyDesktopView::PrivatePlexyDesktopView
{
public:
    PrivatePlexyDesktopView() {}
    ~PrivatePlexyDesktopView()
    {
        mIconViews.clear();
    }

    IconWidgetView* addDirectory(const QString &path);

    QList<IconWidgetView *> mIconViews;
    PlexyGraphicsScene *mScene;
};

PlexyDesktopView::PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent) :
    PlexyDesk::AbstractDesktopView(parent_scene, parent),
    d(new PrivatePlexyDesktopView)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    IconWidgetView *view = d->addDirectory("");

    scene()->addItem(view);
    view->show();
    setAcceptDrops(true);

}

void PlexyDesktopView::dropEvent(QDropEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    event->acceptProposedAction();
}

void PlexyDesktopView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    event->acceptProposedAction();
}

void PlexyDesktopView::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    event->accept();
}

IconWidgetView *PlexyDesktopView::PrivatePlexyDesktopView::addDirectory(const QString &path)
{
    //TODO Write a Stack Widget which can stack IconWidgetViews'
    IconWidgetView *view = new IconWidgetView(QRectF(0.0, 0.0, 480.0, 320.0));

    view->enableDefaultBackground(true);
    view->setDirectoryPath(QDir::homePath() + QLatin1String("/Desktop/"));

    mIconViews.append(view);

    return view;
}
