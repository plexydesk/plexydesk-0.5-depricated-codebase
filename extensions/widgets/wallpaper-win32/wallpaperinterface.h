#ifndef WALLPAPER_INTERFACE_H_
#define WALLPAPER_INTERFACE_H_

#include <abstractplugininterface.h>
#include <datainterface.h>

#include "config.h"

class WALLPAPERCHANGE_EXPORT WallpaperInterface : public PlexyDesk::DataInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

public:
    WallpaperInterface(QObject *parent = 0);
    virtual ~WallpaperInterface();

    PlexyDesk::DataPlugin *instance();
};

#endif // WALLPAPER_INTERFACE_H_
