#ifndef CLASSICBACKGROUNDRENDER_H
#define CLASSICBACKGROUNDRENDER_H


#include <plexy.h>

#include <QImage>

#include <abstractdesktopwidget.h>

class ClassicBackgroundRender : public PlexyDesk::AbstractDesktopWidget
{
    Q_OBJECT
public:
    explicit ClassicBackgroundRender(const QRectF &rect, QGraphicsObject *parent = 0, const QImage &background_image = QImage());

    void setBackgroundImage(const QString &path);

    virtual void paintRotatedView(QPainter *painter, const QRectF &rect);
    virtual void paintFrontView(QPainter *painter, const QRectF &rect);
    virtual void paintDockView(QPainter *painter, const QRectF &rect);
    virtual void paintEditMode(QPainter *painter, const QRectF &rect);

private:
    QImage mBackgroundImage;

};

#endif // CLASSICBACKGROUNDRENDER_H
