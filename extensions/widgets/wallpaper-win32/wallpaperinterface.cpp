#include "wallpaperinterface.h"
#include "wallpaper.h"

#include <QtPlugin>

WallpaperInterface::WallpaperInterface(QObject *parent)
{

}

WallpaperInterface::~WallpaperInterface()
{

}

PlexyDesk::DataPlugin *WallpaperInterface::instance()
{
    return new WallpaperChange(this);
}

Q_EXPORT_PLUGIN2(WallpaperInterface, WallpaperInterface)
