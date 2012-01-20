

#include <desktopwidget.h>

#ifndef PLEXYDESK_QML_DESKTOP_WIDGET_H
#define PLEXYDESK_QML_DESKTOP_WIDGET_H

namespace PlexyDesk
{
class QmlDesktopWidget : public DesktopWidget
{
    Q_OBJECT

public:
    QmlDesktopWidget(const QRectF &rect, QWidget *embeddedWidget = 0, QDeclarativeItem *parent = 0);

    virtual ~QmlDesktopWidget();

    QDeclarativeEngine *engine() const;

    void setSourceUrl(const QUrl &url);

public Q_SLOTS:
    void onQuit();
    
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    class PrivateQmlDesktopWidget;
    PrivateQmlDesktopWidget *const d;
};
} // namespace PlexyDesk
#endif //PLEXYDESK_QML_DESKTOP_WIDGET_H
