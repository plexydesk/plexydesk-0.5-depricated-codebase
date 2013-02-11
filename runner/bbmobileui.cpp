#include "bbmobileui.h"

#include <themepackloader.h>
#include <controllerinterface.h>
#include <pluginloader.h>

#include <bb/cascades/AbstractPane>

using namespace PlexyDesk;

class BBMobileUi::BBMobileUiPrivate
{
public:
    BBMobileUiPrivate(){}
    ~BBMobileUiPrivate(){}

    PlexyDesk::ThemepackLoader *mThemeLoader;
    QMap<QString, ControllerPtr> mControllerMap;
};

BBMobileUi::BBMobileUi(int &argc, char **argv) :
    Application(argc, argv),
    d(new BBMobileUiPrivate)
{
    d->mThemeLoader = new PlexyDesk::ThemepackLoader("default", this);
}

BBMobileUi::~BBMobileUi()
{
    delete d;
}

void BBMobileUi::setupUI()
{
    QString sessionData = d->mThemeLoader->loadSessionFromDisk();
    qDebug() << Q_FUNC_INFO << "==========================" << sessionData;

    QStringList widgets = d->mThemeLoader->desktopWidgets();

    Q_FOREACH(const QString &widget, widgets) {
        addController(widget);
        QSharedPointer<PlexyDesk::ControllerInterface> controller = d->mControllerMap[widget];
        if (controller.data())
            controller->firstRun();

        AbstractPane *view = qobject_cast<AbstractPane*> (controller->defaultView());
        if (view)
            setScene (view);
        else
            qDebug() << Q_FUNC_INFO << "Error loading controller";

    }

}

void BBMobileUi::addController(const QString &controllerName)
{
    QSharedPointer<ControllerInterface> controller =
            (PlexyDesk::PluginLoader::getInstance()->controller(controllerName));

    if (!controller.data()) {
        qWarning() << Q_FUNC_INFO << "Error loading extension" << controllerName;
        return;
    }
    d->mControllerMap[controllerName] = controller;

    controller->setViewport(this);
    controller->setControllerName(controllerName);
}

