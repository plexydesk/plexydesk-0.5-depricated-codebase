#ifndef FLICKER_DATA_H
#define FLICKER_DATA_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>
#include <QtNetwork>

class VISIBLE_SYM FlickerData : public QObject , public PlexyDesk::AbstractPluginInterface 
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
    signals:
         void data(QVariant&);

    private:
    QPixmap bg;
    QImage img;
    QBrush paint;
    QHttp *http;
    QStringList images;
    int requestID;
    int dataID;
    int width;
    int height;
};


#endif
