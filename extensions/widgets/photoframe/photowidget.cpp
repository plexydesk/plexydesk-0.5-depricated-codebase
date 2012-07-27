#include "photowidget.h"

PhotoWidget::PhotoWidget(const QRectF &rect) :
    PlexyDesk::DesktopWidget(rect)
{
}

PhotoWidget::~PhotoWidget()
{
}

void PhotoWidget::paintRotatedView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintRotatedView(painter, rect);
}

void PhotoWidget::paintFrontView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
}

void PhotoWidget::paintDockView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintDockView(painter, rect);
}
