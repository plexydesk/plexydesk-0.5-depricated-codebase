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
    d->mThemeLoader = new PlexyDesk::ThemepackLoader("default", this);

    QStringList widgets = d->mThemeLoader->desktopWidgets();

    Q_FOREACH(const QString &widget, widgets) {
        addController(widget);
    }
}

PlexyDesktopView::~PlexyDesktopView()
{
    delete d;
}

void PlexyDesktopView::layout()
{
    Q_FOREACH(const QString &controllerName, currentControllers()) {

        QRectF rect = d->mThemeLoader->positionForWidget(controllerName, this);

        PlexyDesk::ControllerInterface *controllerPtr = controllerByName(controllerName);
        if (controllerPtr) {
            controllerPtr->setViewRect(rect);
        }
    }
}
