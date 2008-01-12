#ifndef PLEXY_CLASSIC_BACKDROP_H
#define PLEXY_CLASSIC_BACKDROP_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>


class VISIBLE_SYM ClassicBackdrop : public QObject , public PlexyDesk::BackdropInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::BackdropInterface)

    public:
    ClassicBackdrop(QObject * object = 0);
    virtual ~ClassicBackdrop();
    virtual QGraphicsItem * backdrop();// {};
    virtual void render(QPainter *p,QRectF r);// {};
    private:
    QPixmap bg;
    QBrush paint;
    int width;
    int height;
};


#endif
