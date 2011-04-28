#include "wallpaperinterface.h"
#include "wallpaper.h"

#include <QtPlugin>

WallpaperInterface::WallpaperInterface(QObject *parent)
{

}

WallpaperInterface::~WallpaperInterface()
{

}

PlexyDesk::WidgetPlugin *WallpaperInterface::instance()
{
    return new WallpaperChange(this);
}

Q_EXPORT_PLUGIN2(WallpaperInterface, WallpaperInterface)
