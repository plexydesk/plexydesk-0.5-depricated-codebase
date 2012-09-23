#include "photowidget.h"

#include <svgprovider.h>

PhotoWidget::PhotoWidget(const QRectF &rect) :
    PlexyDesk::DesktopWidget(rect)
{
    mShadowEffect = new QGraphicsDropShadowEffect(this);
    mShadowEffect->setBlurRadius(16);
    mShadowEffect->setXOffset(0);
    mShadowEffect->setYOffset(0);
    mShadowEffect->setColor(QColor(0.0, 0.0, 0.0));
    this->setGraphicsEffect(mShadowEffect);
}

PhotoWidget::~PhotoWidget()
{
}

void PhotoWidget::setContentImage(const QPixmap &pixmap)
{
    mContentPixmap = pixmap;
    QRectF pixmapRect = pixmap.rect();

    /*calculate the new height for the content rect width */
    float pixmapHeight = (pixmapRect.height() / pixmapRect.width() ) * contentRect().width();
    pixmapRect.setHeight(pixmapHeight);
    pixmapRect.setWidth(contentRect().width());
    setContentRect(pixmapRect);

    update();
}

void PhotoWidget::paintRotatedView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintRotatedView(painter, rect);
}

void PhotoWidget::paintFrontView(QPainter *painter, const QRectF &rect)
{
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setPen(QColor(255, 255, 255));

    painter->setOpacity(1.0);

    if (mContentPixmap.isNull()) {
        PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
        painter->drawText(QRect(0, 0, 190, 180), Qt::AlignCenter | Qt::AlignRight,
                          QLatin1String("Drag and Drop a Photo Here!"));
    } else {
        //experimental photo frame around the image
        //painter->fillRect(rect.x(), rect.y(), rect.width(), rect.height(),  QColor(255, 255, 255));
        //float frameWidth = 4.0;
        //painter->translate(frameWidth, frameWidth);
        //painter->drawPixmap(rect.x(), rect.y(), rect.width() - (frameWidth * 2), rect.height() - (frameWidth * 2), mContentPixmap);

        painter->drawPixmap(rect.x(), rect.y(), rect.width(), rect.height(), mContentPixmap);
    }
}

void PhotoWidget::paintDockView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintDockView(painter, rect);
}
