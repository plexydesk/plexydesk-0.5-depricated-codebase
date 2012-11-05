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
    bool mHasSession;
};

PlexyDesktopView::PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent) :
    PlexyDesk::AbstractDesktopView(parent_scene, parent),
    d(new PrivatePlexyDesktopView)
{
    d->mThemeLoader = new PlexyDesk::ThemepackLoader("default", this);
    d->mHasSession = false;

    QString sessionData = d->mThemeLoader->loadSessionFromDisk();

    //add the background controller
    setBackgroundController(d->mThemeLoader->desktopBackgroundController());

    if (sessionData.isNull() || sessionData.isEmpty()) {
        QStringList widgets = d->mThemeLoader->desktopWidgets();

        Q_FOREACH(const QString &widget, widgets) {
            addController(widget);
            QSharedPointer<PlexyDesk::ControllerInterface> controller = controllerByName(widget);
            if (controller.data())
                controller->firstRun();
        }
    } else {
      restoreViewFromSession(sessionData);
      d->mHasSession = true;
    }

    connect(this, SIGNAL(sessionUpdated(QString)), this, SLOT(onSessionUpdated(QString)));
}

PlexyDesktopView::~PlexyDesktopView()
{
    delete d;
}

void PlexyDesktopView::layout(const QRectF &screen_rect)
{
    Q_FOREACH(const QString &controllerName, currentControllers()) {
        qDebug() << Q_FUNC_INFO << screen_rect;
        QRectF rect = d->mThemeLoader->positionForWidget(controllerName, screen_rect);
        setControllerRect(controllerName, rect);
    }
}

void PlexyDesktopView::onSessionUpdated(const QString &data)
{
    qDebug() << Q_FUNC_INFO << data;
    d->mThemeLoader->saveSessionToDisk(data);
}
