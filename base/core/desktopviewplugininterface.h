#ifndef DESKTOPVIEWPLUGININTERFACE_H
#define DESKTOPVIEWPLUGININTERFACE_H

#include <plexy.h>

#include <QSharedPointer>
#include <QObject>

#include <abstractdesktopview.h>
#include <desktopviewplugin.h>

namespace PlexyDesk {

class DesktopViewPluginInterface
{
public:
    DesktopViewPluginInterface() {}
    virtual ~DesktopViewPluginInterface() {}

    virtual QSharedPointer<DesktopViewPlugin> view() = 0;
};

}

Q_DECLARE_INTERFACE(PlexyDesk::DesktopViewPluginInterface, "org.plexydesk.DesktopViewPluginInterface")
#endif // DESKTOPVIEWPLUGININTERFACE_H
