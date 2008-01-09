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
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
              //  setCacheMode(QGraphicsView::CacheNone);
                    setViewportUpdateMode(FullViewportUpdate);
                        setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
                       centerOn(0,0);
                       setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

DesktopView::~DesktopView()
{
    delete d;
}


}
#include "desktopview.moc"
