#ifndef FLICKER_DATA_H
#define FLICKER_DATA_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>
#include <QtNetwork>

class VISIBLE_SYM FlickerData :public PlexyDesk::AbstractPluginInterface 
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    FlickerData(QObject * object = 0);
    virtual ~FlickerData();
    void init();
    virtual QGraphicsItem * item();// {};
    virtual void render(QPainter *p,QRectF r);// {};
    public slots:
          void loadImages(int, bool);
          void nextImage();
    signals:
         void data(QVariant&);

    private:
    QPixmap bg;
    QImage img;
    QBrush paint;
    QHttp *http;
    QImage newWall;
    QStringList images;
    int requestID;
    int dataID;
    int slideCount;
    int currentSlide;
    QTimer * imageTimer;
};


#endif
