#ifndef PLEXY_CLASSIC_BACKDROP_H
#define PLEXY_CLASSIC_BACKDROP_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>


class VISIBLE_SYM ClassicBackdrop : public QObject , public PlexyDesk::AbstractPluginInterface 
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    ClassicBackdrop(QObject * object = 0);
    virtual ~ClassicBackdrop();
    void init(){}
    virtual QGraphicsItem * item();// {};
    void render(QPainter *p,QRectF r);// {};
    private:
    QPixmap bg;
    QImage img;
    QBrush paint;
    int width;
    int height;
};


#endif
