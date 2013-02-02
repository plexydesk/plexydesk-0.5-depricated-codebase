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
#include <nativestyle.h>
#include <windowbutton.h>

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
    QTimer *mPressHoldTimer;
    AbstractDesktopWidget::State mWidgetState;
    QPixmap mDefaultBackgroundPixmap;

    QRectF saveRect;
    QPointF clickPos;
    bool mDefaultBackground;
    bool mHasDefaultBackground;
    bool mDockMode;
    bool mWindowMode;
    bool mEditMode;

    // image cache
    SvgProvider *mSvgRender;

    QPropertyAnimation *mPropertyAnimationForZoom;
    QPropertyAnimation *mPropertyAnimationForRotation;

    QGraphicsDropShadowEffect *mShadowEffect;
    Style *mStyle;
    QString mWindowTitle;
    WindowButton *mCloseButton;
};

DesktopWidget::DesktopWidget(const QRectF &rect, QGraphicsObject *parent)
    : AbstractDesktopWidget(rect, parent), d(new PrivateDesktopWidget)
{
    d->mBoundingRect = rect;
    d->mEditMode = false;
    d->mDefaultBackground = true;
    d->saveRect = rect;
    d->mWidgetState = VIEW;
    d->mHasDefaultBackground = false;
    d->mStyle = 0;
    d->mDockMode = true;
    d->mWindowMode = true;

    setStyle(new NativeStyle(this));

    d->mPropertyAnimationForZoom = new QPropertyAnimation(this);
    d->mPropertyAnimationForZoom->setTargetObject(this);

    d->mPropertyAnimationForRotation = new QPropertyAnimation(this);
    d->mPropertyAnimationForRotation->setTargetObject(this);
    d->mPropertyAnimationForRotation->setPropertyName("rotation");
    d->mPropertyAnimationForRotation->setDuration(800);
    d->mPropertyAnimationForRotation->setStartValue(0);
    d->mPropertyAnimationForRotation->setEndValue(180);
    d->mPropertyAnimationForRotation->setEasingCurve(QEasingCurve::InSine);

    connect(d->mPropertyAnimationForZoom, SIGNAL(finished()), this, SLOT(propertyAnimationForZoomDone()));
    connect(d->mPropertyAnimationForRotation, SIGNAL(finished()), this, SLOT(propertyAnimationForRotationDone()));

    d->mSvgRender = new SvgProvider();
    //setDefaultImages();

    //setCacheMode(QGraphicsItem::ItemCoordinateCache, d->mDefaultBackgroundPixmap.size());
    setCacheMode(DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setAcceptsHoverEvents(true);

    //presshold
    d->mPressHoldTimer = new QTimer(this);
    connect(d->mPressHoldTimer, SIGNAL(timeout()), this, SLOT(pressHoldTimeOut()));

    //dropshadow
    d->mShadowEffect = new QGraphicsDropShadowEffect(this);
    d->mShadowEffect->setBlurRadius(16);
    d->mShadowEffect->setXOffset(0);
    d->mShadowEffect->setYOffset(0);
    d->mShadowEffect->setColor(QColor(0.0, 0.0, 0.0));
    this->setGraphicsEffect(d->mShadowEffect);

    //window buttons
    d->mCloseButton = new WindowButton(this);
    d->mCloseButton->show();
#ifdef Q_WS_QPA
    d->mCloseButton->setSize(QSize(48, 48));
#else
    d->mCloseButton->setSize(QSize(12, 12));
#endif
    d->mCloseButton->setPos(rect.x() + 10, rect.y() + 6);
    connect(d->mCloseButton, SIGNAL(clicked()), this, SLOT(windowCloseButtonClicked()));
}

DesktopWidget::~DesktopWidget()
{
    qDebug() << Q_FUNC_INFO;
    delete d;
}

void DesktopWidget::enableDefaultBackground(bool enable)
{

    if (!d->mHasDefaultBackground) {
        //setDefaultImages();
    }
    if (!enable) {
        d->mCloseButton->hide();
    }
    d->mDefaultBackground = enable;
}

void DesktopWidget::enableShadow(bool enable)
{
    if(!enable) {
        d->mShadowEffect->setBlurRadius(0);
    } else {
        d->mShadowEffect->setBlurRadius(16);
    }
}

void DesktopWidget::enableDockMode(bool enable)
{
    d->mDockMode = enable;
}

void DesktopWidget::enableWindowMode(bool enable)
{
    d->mWindowMode = enable;

    if (enable && d->mCloseButton) {
        d->mCloseButton->show();
    } else if (d->mCloseButton) {
        d->mCloseButton->hide();
    }
}

void DesktopWidget::setDefaultImages()
{
    if (boundingRect().width() == 0 || boundingRect().height() == 0) {
        d->mHasDefaultBackground = false;
        return;
    }
    d->mDefaultBackgroundPixmap = genDefaultBackground(boundingRect().width(), boundingRect().height());
    d->mHasDefaultBackground = true;
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

    QImage canvas (dock_rect.width(), dock_rect.height(), QImage::Format_ARGB32_Premultiplied);

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
    if (!d->mDefaultBackground) {
        return;
    }

    if (!d->mStyle) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(rect.x() , rect.y() , d->mDefaultBackgroundPixmap.width() / scaleFactorForWidth(), d->mDefaultBackgroundPixmap.height() / scaleFactorForHeight()), d->mDefaultBackgroundPixmap);
        p->restore();
    } else {
        StyleFeatures feature;
        feature.exposeRect = rect;
        feature.state = StyleFeatures::SF_FrontView;
        d->mStyle->paintControlElement(Style::CE_Frame, feature, p);
    }
}

void DesktopWidget::paintFrontView(QPainter *p, const QRectF &rect)
{
    if (!d->mDefaultBackground)
        return;

    if (!d->mStyle) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(rect.x() , rect.y() , d->mDefaultBackgroundPixmap.width() / scaleFactorForWidth(), d->mDefaultBackgroundPixmap.height() / scaleFactorForHeight()), d->mDefaultBackgroundPixmap);
        p->restore();
    } else {
        StyleFeatures feature;
        feature.exposeRect = rect;
        feature.state = StyleFeatures::SF_FrontView;
        d->mStyle->paintControlElement(Style::CE_Frame, feature, p);
    }
}

void DesktopWidget::paintDockView(QPainter *p, const QRectF &rect)
{
    if (!d->mEditMode) {

        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        ///p->drawPixmap(QRect(rect.x(), rect.y(), rect.width(), rect.height()), d->mDefaultBackgroundPixmap);

        if (!d->mStyle) {
            p->save();
            p->setRenderHints(QPainter::SmoothPixmapTransform);
            p->drawPixmap(QRect(rect.x() , rect.y() , d->mDefaultBackgroundPixmap.width() / scaleFactorForWidth(), d->mDefaultBackgroundPixmap.height() / scaleFactorForHeight()), d->mDefaultBackgroundPixmap);
            p->restore();
        } else {
            StyleFeatures feature;
            feature.exposeRect = rect;
            feature.state = StyleFeatures::SF_FrontView;
            d->mStyle->paintControlElement(Style::CE_Frame, feature, p);
        }

        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(rect.x(), rect.y(), rect.width(), rect.height()), Qt::AlignCenter, label());
        p->restore();
    }
}

void DesktopWidget::paintEditMode(QPainter *p, const QRectF &rect)
{
    if (d->mEditMode) {
        p->save();
        p->setRenderHints(QPainter::SmoothPixmapTransform);
        p->drawPixmap(QRect(0, 0, rect.width(), rect.height()), d->mDefaultBackgroundPixmap);
        p->setPen(QColor(255, 255, 255));
        p->drawText(QRect(8, 5, 64, 64), Qt::AlignCenter, QLatin1String ("Close"));
        p->restore();
    }
}

void DesktopWidget::pressHoldTimeOut()
{
    d->mEditMode = true;
    update();
}

void DesktopWidget::windowCloseButtonClicked()
{
    qDebug() << Q_FUNC_INFO;
    this->hide();

    Q_EMIT closed(this);
}

void DesktopWidget::zoomDone()
{
    prepareGeometryChange();
    resetMatrix();
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

void DesktopWidget::propertyAnimationForRotationDone()
{
    setChildWidetVisibility(true);
}

void DesktopWidget::configState(AbstractDesktopWidget::State s)
{
    if (s == d->mWidgetState) {
        return ;
    }

    resetMatrix();
    prepareGeometryChange();

    if (s == DOCKED) {
        setContentRect(dockRect());
    } else {
        setContentRect(contentRect());
    }

    d->mWidgetState = s;

    if (d->mWidgetState == DOCKED) {
        setState(VIEW);
        prepareGeometryChange();
        this->setContentRect(contentRect());
    } else {
        setState(DOCKED);
        prepareGeometryChange();
        this->setContentRect(dockRect());
    }
}

void DesktopWidget::setStyle(Style *style)
{
    d->mStyle = style;
}

void DesktopWidget::setWindowTitle(const QString &title)
{
    d->mWindowTitle = title;
}

void DesktopWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO << event->pos();
    d->mPressHoldTimer->stop();
    Q_EMIT clicked();
    AbstractDesktopWidget::mouseReleaseEvent(event);
}

void DesktopWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton && (state() == VIEW || state() == ROTATED)) {
        this->setChildWidetVisibility(false);
        d->mPropertyAnimationForRotation->start();
        AbstractDesktopWidget::mousePressEvent(event);
        //QGraphicsItem::mousePressEvent(event);
        return;
    }

    if (d->mEditMode) {
        d->mPressHoldTimer->stop();
        Q_EMIT closed(this);
        //QGraphicsItem::mousePressEvent(event);
        return;
    }

    if (event->buttons() == Qt::LeftButton && state() == DOCKED) {
        d->mPressHoldTimer->start(3000);
        qDebug() << Q_FUNC_INFO << "Press start";
        AbstractDesktopWidget::mousePressEvent(event);
        //QGraphicsItem::mousePressEvent(event);
        return;
    }
    AbstractDesktopWidget::mousePressEvent(event);
}
void DesktopWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!d->mDockMode) {
        QGraphicsItem::mouseDoubleClickEvent(event);
        return;
    }

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
        d->mPropertyAnimationForZoom->setDuration(200);
        QRectF startRect ((contentRect().width() / 2) - (dockRect().width() / 2),
                        (contentRect().height() / 2) -(dockRect().height() /2),
                        dockRect().width(), dockRect().height());
        d->mPropertyAnimationForZoom->setStartValue(startRect);
        d->mPropertyAnimationForZoom->setEndValue(contentRect());
        d->mPropertyAnimationForZoom->setEasingCurve (QEasingCurve::InQuart);

        d->mPropertyAnimationForZoom->start();
        setChildWidetVisibility(true);

    } else {
        prepareGeometryChange();
        d->mPropertyAnimationForZoom->setPropertyName("rect");
        d->mPropertyAnimationForZoom->setDuration(100);

        QRectF endRect ((contentRect().width() / 2) - (dockRect().width() / 2),
                        (contentRect().height() / 2) -(dockRect().height() /2),
                        dockRect().width(), dockRect().height());
        d->mPropertyAnimationForZoom->setEndValue(endRect);
        d->mPropertyAnimationForZoom->setStartValue(contentRect());
        d->mPropertyAnimationForZoom->setEasingCurve (QEasingCurve::OutQuart);
        this->setVisible(true);
        d->mPropertyAnimationForZoom->start();
        setChildWidetVisibility(false);
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}
}
