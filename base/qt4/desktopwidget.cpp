#include "desktopwidget.h"

namespace PlexyDesk
{

DesktopWidget::DesktopWidget(const QRectF &rect, QWidget *embeddedWidget, QDeclarativeItem *parent)
    :AbstractDesktopWidget(rect, embeddedWidget, parent)
{
}

DesktopWidget::~DesktopWidget()
{
}


}

