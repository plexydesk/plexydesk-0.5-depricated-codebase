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
    QTimer *spintimer;
    QTimer *mPressHoldTimer;
    AbstractDesktopWidget::State mWidgetState;
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

    QPropertyAnimation *mPropertyAnimationForZoom;
};

DesktopWidget::DesktopWidget(const QRectF &rect, QWidget *widget, QGraphicsObject *parent)
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
    d->mWidgetState = VIEW;
    d->angle = 0;
    d->angleHide = 0;
    d->scale = 1;
    d->mPropertyAnimationForZoom = new QPropertyAnimation(this);
    d->mPropertyAnimationForZoom->setTargetObject(this);

    connect(d->mPropertyAnimationForZoom, SIGNAL(finished()), this, SLOT(propertyAnimationForZoomDone()));

    d->mSvgRender = new SvgProvider();
    d->mCache = new ImageCache();
    setDefaultImages();

    setCacheMode(QGraphicsItem::ItemCoordinateCache, d->panel.size());
    setCacheMode(DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setAcceptsHoverEvents(true);

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
        p->drawPixmap(QRect(rect.x(), rect.y(), rect.width(), rect.height()), d->dock);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(rect.x(), rect.y(), rect.width(), rect.height()), Qt::AlignCenter, d->mName);
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

void DesktopWidget::spin()
{
    d->angle += 18;
    setCacheMode(ItemCoordinateCache);
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.rotate(d->angle, Qt::YAxis);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->angle >= 180) {
        if (state() == ROTATED) {
            setState(VIEW);
        } else {
            setState(ROTATED);
        }
        d->spintimer->stop();
        resetMatrix();
        setCacheMode(DeviceCoordinateCache);
        d->angle = 0;
    }
}

void DesktopWidget::pressHoldTimeOut()
{
    d->mEditMode = true;
    update();
}

void DesktopWidget::zoomDone()
{
    prepareGeometryChange();
    resetMatrix();
    d->opacity = 1.0;
}

void DesktopWidget::propertyAnimationForZoomDone()
{
    if (state() == DOCKED) {
        setState (VIEW);
    } else {
        setState(DOCKED);
    }

    zoomDone();
}

void DesktopWidget::zoomIn(int frame)
{
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.scale(frame / 150.0, frame / 150.0);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->opacity >= 0.0) {
        //d->opacity -= 0.3;
    }
}
void DesktopWidget::zoomOut(int frame)
{
    QPointF center = boundingRect().center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.scale(1 - frame / 450.0, 1 - frame / 450.0);
    mat.translate(-center.x(), -center.y());
    setTransform(mat);
    if (d->opacity >= 0.0) {
        //d->opacity -= 0.2;
    }
}


void DesktopWidget::configState(AbstractDesktopWidget::State s)
{
    if (s == d->mWidgetState) {
        return ;
    }

    qDebug() << Q_FUNC_INFO ;
    resetMatrix();
    prepareGeometryChange();

    if (s == DOCKED) {
        setRect(dockRect());
    } else {
        setRect(contentRect());
    }

    d->mWidgetState = s;
    if (d->proxyWidget) {
        d->proxyWidget->hide();
    }

    if (d->mWidgetState == DOCKED) {
        setState(VIEW);
        prepareGeometryChange();
        this->setRect(contentRect());
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
    } else {
        setState(DOCKED);
        prepareGeometryChange();
        this->setRect(dockRect());
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
    }
}

void DesktopWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    d->mPressHoldTimer->stop();
    qDebug() << Q_FUNC_INFO << "STOP timer";
    QGraphicsObject::mouseReleaseEvent(event);
}

void DesktopWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton && (state() == VIEW || state() == ROTATED)) {
        d->spintimer->start(36);
        return;
    }

    if (d->mEditMode) {
        d->mPressHoldTimer->stop();
        Q_EMIT closed();
        return;
    }

    if (event->buttons() == Qt::LeftButton && state() == DOCKED) {
        d->mPressHoldTimer->start(3000);
        qDebug() << Q_FUNC_INFO << "Press start";
        return;
    }
}
void DesktopWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) {
        /* We will let the widgets decide what to do with
           left clicks
           */
        QGraphicsItem::mouseDoubleClickEvent(event);
        return;
    }
    if (state() == AbstractDesktopWidget::DOCKED) {
        prepareGeometryChange();
        //TODO: Use Animation forward and back methods
        d->mPropertyAnimationForZoom->setPropertyName("rect");
        d->mPropertyAnimationForZoom->setDuration(100);
        QRectF startRect ((contentRect().width() / 2) - (dockRect().width() / 2),
                        (contentRect().height() / 2) -(dockRect().height() /2),
                        dockRect().width(), dockRect().height());
        d->mPropertyAnimationForZoom->setStartValue(startRect);
        d->mPropertyAnimationForZoom->setEndValue(contentRect());
        d->mPropertyAnimationForZoom->setEasingCurve (QEasingCurve::InBounce);
        this->setRect(d->saveRect);
        if (d->proxyWidget) {
            d->proxyWidget->show();
        }
        d->mPropertyAnimationForZoom->start();

    } else {
        d->mPropertyAnimationForZoom->setPropertyName("rect");
        d->mPropertyAnimationForZoom->setDuration(100);

        QRectF endRect ((contentRect().width() / 2) - (dockRect().width() / 2),
                        (contentRect().height() / 2) -(dockRect().height() /2),
                        dockRect().width(), dockRect().height());
        d->mPropertyAnimationForZoom->setEndValue(endRect);
        d->mPropertyAnimationForZoom->setStartValue(contentRect());
        d->mPropertyAnimationForZoom->setEasingCurve (QEasingCurve::OutBounce);
        prepareGeometryChange();
        this->setRect(dockRect());
        if (d->proxyWidget) {
            d->proxyWidget->hide();
        }
        this->setVisible(true);
        d->mPropertyAnimationForZoom->start();
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}
}
