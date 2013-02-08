#include "plexydesktopview.h"

#include <QGraphicsItem>
#include <QGraphicsScene>

#include <datasource.h>
#include <pluginloader.h>
#include <themepackloader.h>
#include <button.h>

#include "fileiconwidget.h"

class PlexyDesktopView::PrivatePlexyDesktopView
{
public:
    PrivatePlexyDesktopView() {}
    ~PrivatePlexyDesktopView() {}

    PlexyDesk::ThemepackLoader *mThemeLoader;
    bool mHasSession;
    QMenu *mMenu;

    PlexyDesk::DesktopWidget *mParentWidget;
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
      restoreViewFromSession(sessionData, 1);
      d->mHasSession = true;
    }

    d->mMenu = new QMenu (this);
    this->createActions();

    connect(this, SIGNAL(sessionUpdated(QString)), this, SLOT(onSessionUpdated(QString)));
}

PlexyDesktopView::~PlexyDesktopView()
{
    delete d;
}

void PlexyDesktopView::layout(const QRectF &screen_rect)
{
#ifdef Q_WS_QPA
    this->createMobileActions();
#endif
    if (d->mHasSession)
        return;

    Q_FOREACH(const QString &controllerName, currentControllers()) {
        QRectF rect = d->mThemeLoader->positionForWidget(controllerName, screen_rect);
        setControllerRect(controllerName, rect);
    }
}

void PlexyDesktopView::createActions()
{
    Q_FOREACH(const QString &controllerName, currentControllers()) {
        QMenu *submenu = 0;
        if (controller(controllerName)) {
            PlexyDesk::ControllerPtr contr = controller(controllerName);
            submenu = new QMenu(this);
            connect(submenu, SIGNAL(triggered(QAction*)), this, SLOT(onTriggered(QAction *)));
            Q_FOREACH(const QString &action, contr->actions()) {
                QAction *menuAction = new QAction(this);
                QVariant menuData = controllerName;
                menuAction->setText(action);
                menuAction->setData(menuData);
                submenu->addAction(menuAction);
            }

            if (contr->actions().count() > 0) {
                QString actionLabel = PlexyDesk::PluginLoader::getInstance()->controllerName(controllerName);
                submenu->setTitle(actionLabel);
                d->mMenu->addMenu(submenu);
            } else
                delete submenu;
        }
    }
}

void PlexyDesktopView::createMobileActions()
{
    QRect rect (0.0, 0.0, this->width(), 128);
    d->mParentWidget = new PlexyDesk::DesktopWidget(rect, 0);
    d->mParentWidget->setLabelName("Navigation Menu");
    d->mParentWidget->enableWindowMode(false);

    addWidgetToView(d->mParentWidget);
    d->mParentWidget->show();
    //TODO: Calculate the Z value;
    d->mParentWidget->setZValue(1000);
    d->mParentWidget->setFlag(QGraphicsItem::ItemIsMovable, false);

    float layoutWidth = 10.0;
    Q_FOREACH(const QString &controllerName, currentControllers()) {

        if (controller(controllerName)) {
            PlexyDesk::ControllerPtr contr = controller(controllerName);

            Q_FOREACH(const QString &action, contr->actions()) {
                PlexyDesk::Button *button = new PlexyDesk::Button(d->mParentWidget);
                button->setLabel(action);
                const QVariant data = controllerName;
                button->setActionData(data);
                button->show();
                button->setSize(QSize(242,108));
                button->setPos(layoutWidth, 10.0);
                layoutWidth += 10.0 + button->boundingRect().width();
                connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
            }
        }
    }

    d->mParentWidget->setPos(0.0, this->height() - d->mParentWidget->boundingRect().height());
}

void PlexyDesktopView::onSessionUpdated(const QString &data)
{
    d->mThemeLoader->saveSessionToDisk(data);
}

void PlexyDesktopView::onWidgetClosed(PlexyDesk::AbstractDesktopWidget *widget)
{
    PlexyDesk::AbstractDesktopView::onWidgetClosed(widget);
}

void PlexyDesktopView::onTriggered(QAction *action)
{
    QString controllerName = action->data().toString();
    QString actionName = action->text();

    if (controller(controllerName)) {
        PlexyDesk::ControllerPtr contr = controller(controllerName);
        QVariantMap map;
        contr->requestAction(actionName, map);
    }
}

void PlexyDesktopView::onButtonClicked()
{
    PlexyDesk::Button *button = qobject_cast<PlexyDesk::Button *>(sender());

    if (button) {
        QString controllerName = button->actionData().toString();
        QString action = button->label();

        if (controller(controllerName)) {
            PlexyDesk::ControllerPtr contr = controller(controllerName);
            QVariantMap map;
            contr->requestAction(action, map);
        }
    }
}

void PlexyDesktopView::contextMenuEvent(QContextMenuEvent *event)
{
    QList<QGraphicsItem *> items = scene()->items(event->pos());

    if (items.count() == 1 && d->mMenu) {
        d->mMenu->popup(event->globalPos());
    }
}
