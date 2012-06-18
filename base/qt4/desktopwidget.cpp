#include "desktopwidget.h"

#include <QCoreApplication>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QTimeLine>
#include <QTimer>
#include <QDir>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>

#include <imagecache.h>
#include <svgprovider.h>

namespace PlexyDesk
{

class DesktopWidget::PrivateDesktopWidget
{
public:
    PrivateDesktopWidget()
    {
    }
    ~PrivateDesktopWidget()
    {
    }

    QRectF mBoundingRect;
    QTimeLine *zoomin;
    QTimeLine *zoomout;
    QTimer *spintimer;
    QTimer *mPressHoldTimer;
    AbstractDesktopWidget::State s;
    QPixmap panel;
    QPixmap back;
    QPixmap dock;
    int angle;
    int angleHide;
    QGraphicsProxyWidget *proxyWidget;
    double opacity;
    QRectF saveRect;
    int scale;
    QPointF clickPos;
    bool backdrop;
    bool mEditMode;
    QString mName;

    // image cache
    ImageCache *mCache;
    SvgProvider *mSvgRender;
};

DesktopWidget::DesktopWidget(const QRectF &rect, QWidget *widget, QDeclarativeItem *parent)
    : AbstractDesktopWidget(rect, widget, parent), d(new PrivateDesktopWidget)
{
    d->proxyWidget = 0;
    d->mName = QLatin1String ("Widget");
    if (widget) {
        d->proxyWidget = new QGraphicsProxyWidget(this);
        d->proxyWidget->setFocusPolicy(Qt::StrongFocus);
        d->proxyWidget->setWidget(widget);
        d->proxyWidget->show();
    }

    d->mBoundingRect = rect;
    d->mEditMode = false;
    d->backdrop = true;
    d->opacity = 1.0;
    d->saveRect = rect;
    d->s = VIEW;
    d->angle = 0;
    d->angleHide = 0;
    d->scale = 1;


    d->mSvgRender = new SvgProvider();
    d->mCache = new ImageCache();
    setDefaultImages();

    setCacheMode(QGraphicsItem::ItemCoordinateCache, d->panel.size());
    setCacheMode(DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setAcceptsHoverEvents(true);

    // zoom in settings
    d->zoomin = new QTimeLine(150, this);
    d->zoomin->setFrameRange(120, 150);
    connect(d->zoomin, SIGNAL(frameChanged(int)),
         this, SLOT(zoomIn(int)));
    connect(d->zoomin, SIGNAL(finished()),
         this, SLOT(zoomDone()));
    // zoom out
    d->zoomout = new QTimeLine(150, this);
    d->zoomout->setFrameRange(0, 150);
    connect(d->zoomout, SIGNAL(frameChanged(int)),
         this, SLOT(zoomOut(int)));
    connect(d->zoomout, SIGNAL(finished()),
         this, SLOT(zoomDone()));
    d->zoomin->start();
    // spin
    d->spintimer = new QTimer(this);
    connect(d->spintimer, SIGNAL(timeout()), this, SLOT(spin()));

    //presshold
    d->mPressHoldTimer = new QTimer(this);
    connect(d->mPressHoldTimer, SIGNAL(timeout()), this, SLOT(pressHoldTimeOut()));
}

DesktopWidget::~DesktopWidget()
{
    delete d;
}

void DesktopWidget::enableDefaultBackground(bool enable)
{
    d->backdrop = enable;
}


void DesktopWidget::setDefaultImages()
{
    d->dock = genDefaultBackground(72, 72);
    d->panel = genDefaultBackground(boundingRect().width(), boundingRect().height());
    d->back = d->panel;
}

QPixmap DesktopWidget::genDefaultBackground(int w, int h)
{
    if (w == 0 || h == 0) {
        return QPixmap();
    }

    /* widget top */
    QRect dock_rect (0, 0, w, h);
    QRect dock_topleft(0, 0, 10, 10);
    QRect dock_topright((dock_rect.width() - 10), 0, 10, 10);
    QRect dock_top(10, 0, (dock_rect.width() - 20), 10);

    QRect dock_right((dock_rect.width() - 10), 10, 10, (dock_rect.height() - 10));

    QRect dock_bottomright((dock_rect.width() - 10), (dock_rect.height() - 10), 10, 10);
    QRect dock_bottom(10, (dock_rect.height() - 10), (dock_rect.width() - 20), 10);
    QRect dock_bottomleft(0, (dock_rect.height() - 10), 10, 10);

    QRect dock_left (0, 10, 10, (dock_rect.height() - 20));

    QImage canvas (dock_rect.height(), dock_rect.width(), QImage::Format_ARGB32_Premultiplied);

    QPainter svg_painter(&canvas);
    svg_painter.setCompositionMode(QPainter::CompositionMode_Source);
    svg_painter.setBackgroundMode(Qt::TransparentMode);
    svg_painter.setRenderHint(QPainter::SmoothPixmapTransform);
    svg_painter.fillRect(dock_rect, Qt::transparent);

    svg_painter.save();
    svg_painter.setPen (Qt::black);
    svg_painter.drawRect(dock_rect);
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_rect,
            d->mSvgRender->get(QLatin1String("background#center"), dock_rect.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_topleft,
            d->mSvgRender->get(QLatin1String("background#topleft"), dock_topleft.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_topright,
            d->mSvgRender->get(QLatin1String("background#topright"), dock_topleft.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_top,
            d->mSvgRender->get(QLatin1String("background#top"), dock_top.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_right,
            d->mSvgRender->get(QLatin1String("background#right"), dock_right.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_bottomright,
            d->mSvgRender->get(QLatin1String("background#bottomright"), dock_bottomright.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_bottom,
            d->mSvgRender->get(QLatin1String("background#bottom"), dock_bottom.size()));
    svg_painter.restore();

    svg_painter.save(); svg_painter.drawPixmap(dock_bottomleft,
            d->mSvgRender->get(QLatin1String("background#bottomleft"), dock_bottomleft.size()));
    svg_painter.restore();

    svg_painter.save();
    svg_painter.drawPixmap(dock_left,
            d->mSvgRender->get(QLatin1String("background#left"), dock_left.size()));
    svg_painter.restore();

    svg_painter.end();

    return QPixmap::fromImage(canvas);
}


void DesktopWidget::paintRotatedView(QPainter *p, const QRectF &rect)
{
    if (!d->backdrop) {
        return;
    }

    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->back);
    p->restore();
}

void DesktopWidget::paintFrontView(QPainter *p, const QRectF &rect)
{
    if (!d->backdrop)
        return;
    p->save();
    p->setOpacity(0.8);
    p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->drawPixmap(QRect(0, 0, d->panel.width(), d->panel.height()), d->panel);
    p->restore();
}

void DesktopWidget::paintDockView(QPainter *p, const QRectF &rect)
{
    if (!d->mEditMode) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->dock);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(0, 0,72, 72), Qt::AlignCenter, d->mName);
        p->restore();
    }
}

void DesktopWidget::paintEditMode(QPainter *p, const QRectF &rect)
{
    if (d->mEditMode) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->dock);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(8, 5, 64, 64), Qt::AlignCenter, QLatin1String ("Close"));
        p->restore();
    }
}

}
