#include "desktopview.h"


namespace PlexyDesk
{
class  DesktopView::Private
{
    public:
    Private(){}
    ~Private(){}
};

DesktopView::DesktopView ( QGraphicsScene * scene, QWidget * parent ):QGraphicsView(scene,parent),d(new Private)
{
}

DesktopView::~DesktopView()
{
    delete d;
}

void DesktopView::addWallpaper(const QString& path)
{

}

}
#include "desktopview.moc"
