#include "classicbackgroundrender.h"

ClassicBackgroundRender::ClassicBackgroundRender(const QRectF &rect, QGraphicsObject *parent, const QImage &background_image) :
    PlexyDesk::AbstractDesktopWidget(rect, parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    mBackgroundImage = background_image;
}

void ClassicBackgroundRender::setBackgroundImage(const QString &path)
{
    mBackgroundImage.load(path);
    update();
}

void ClassicBackgroundRender::paintRotatedView(QPainter * /*painter*/, const QRectF & /*rect*/)
{
}

void ClassicBackgroundRender::paintFrontView(QPainter *painter, const QRectF & /*rect*/)
{
    painter->drawImage(contentRect(), mBackgroundImage);
}

void ClassicBackgroundRender::paintDockView(QPainter * /*painter*/, const QRectF & /*rect*/)
{
}

void ClassicBackgroundRender::paintEditMode(QPainter * /*painter*/, const QRectF & /*rect*/)
{
}
