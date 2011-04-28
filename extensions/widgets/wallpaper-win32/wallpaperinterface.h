#ifndef WALLPAPER_INTERFACE_H_
#define WALLPAPER_INTERFACE_H_

#include <abstractplugininterface.h>
#include <widgetinterface.h>
#include <widgetplugin.h>

#include "config.h"

class WALLPAPERCHANGE_EXPORT WallpaperInterface : public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public:
    WallpaperInterface(QObject *parent = 0);
    virtual ~WallpaperInterface();

    PlexyDesk::WidgetPlugin *instance();
};

#endif // WALLPAPER_INTERFACE_H_
