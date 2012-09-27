#include "plexydesktopview.h"

#include <datasource.h>
#include <pluginloader.h>
#include <backdropplugin.h>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "fileiconwidget.h"
#include "plexygraphicsscene.h"

#include <themepackloader.h>

class PlexyDesktopView::PrivatePlexyDesktopView
{
public:
    PrivatePlexyDesktopView() {}
    ~PrivatePlexyDesktopView() {}

    PlexyDesk::ThemepackLoader *mThemeLoader;
};

PlexyDesktopView::PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent) :
    PlexyDesk::AbstractDesktopView(parent_scene, parent),
    d(new PrivatePlexyDesktopView)
{
//    addController("folderwidget");
//    addController("plexyclock");
//    addController("photoframe");
}

PlexyDesktopView::~PlexyDesktopView()
{
    delete d;
}

void PlexyDesktopView::layout()
{
    d->mThemeLoader = new PlexyDesk::ThemepackLoader("default", this);

    QStringList widgets = d->mThemeLoader->desktopWidgets();

    qDebug() << Q_FUNC_INFO << widgets;

    Q_FOREACH(QString widget, widgets) {
        addController(widget);
    }

}
