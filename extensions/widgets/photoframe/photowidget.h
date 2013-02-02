#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <plexy.h>
#include <desktopwidget.h>
#include <QGraphicsDropShadowEffect>

class PhotoWidget : public PlexyDesk::DesktopWidget
{
    Q_OBJECT

public:
    explicit PhotoWidget(const QRectF &rect);

    virtual ~PhotoWidget();

    void setContentImage(const QPixmap &pixmap);

private:
    virtual void paintRotatedView(QPainter *painter, const QRectF &rect);

    virtual void paintFrontView(QPainter *painter, const QRectF &rect);

    virtual void paintDockView(QPainter *painter, const QRectF &rect);

    QPixmap mContentPixmap;
    QPixmap mFramePixmap;
    QPixmap mFrameContentPixmap;
};

#endif // PHOTOWIDGET_H
