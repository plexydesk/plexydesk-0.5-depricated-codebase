#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H

#include <plexy.h>
#include <plexyconfig.h>
#include <abstractdesktopwidget.h>

#include <style.h>

#include "plexydeskuicore_global.h"

namespace PlexyDesk
{
class PLEXYDESKUICORE_EXPORT DesktopWidget : public AbstractDesktopWidget
{
    Q_OBJECT

public:
    DesktopWidget(const QRectF &rect, QGraphicsObject *parent = 0);

    virtual ~DesktopWidget();

    void enableDefaultBackground(bool enable);

    void enableShadow(bool enable);

    void enableDockMode(bool enable = true);

    void enableWindowMode(bool enable = true);

    void configState(AbstractDesktopWidget::State s);

    void setStyle(Style *style);

    void setWindowTitle(const QString &title);


public Q_SLOTS:
    void zoomDone();
    void propertyAnimationForZoomDone();
    void propertyAnimationForRotationDone();
    void pressHoldTimeOut();
    void windowCloseButtonClicked();

Q_SIGNALS:
    void clicked();

protected:
    virtual void paintRotatedView(QPainter *painter, const QRectF &rect);
    virtual void paintFrontView(QPainter *painter, const QRectF &rect);
    virtual void paintDockView(QPainter *painter, const QRectF &rect);
    virtual void paintEditMode(QPainter *painter, const QRectF &rect);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void setDefaultImages();
    QPixmap genDefaultBackground(int width, int height);

    class PrivateDesktopWidget;
    PrivateDesktopWidget *const d;
};

}
#endif // DESKTOPWIDGET_H
