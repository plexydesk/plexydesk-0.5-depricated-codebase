#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H

#include <plexy.h>
#include <plexyconfig.h>
#include <abstractdesktopwidget.h>

namespace PlexyDesk
{
class PLEXYDESK_EXPORT DesktopWidget : public AbstractDesktopWidget
{
    Q_OBJECT

public:
    DesktopWidget(const QRectF &rect, QWidget *embeddedWidget = 0, QDeclarativeItem *parent = 0);
    virtual ~DesktopWidget();
};

}
#endif // DESKTOPWIDGET_H
