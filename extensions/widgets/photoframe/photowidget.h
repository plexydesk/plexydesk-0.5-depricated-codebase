#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <plexy.h>
#include <desktopwidget.h>

class PhotoWidget : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    explicit PhotoWidget(const QRectF &rect);
    virtual ~PhotoWidget();

private:
    virtual void paintRotatedView(QPainter *painter, const QRectF &rect);
    virtual void paintFrontView(QPainter *painter, const QRectF &rect);
    virtual void paintDockView(QPainter *painter, const QRectF &rect);
};

#endif // PHOTOWIDGET_H
