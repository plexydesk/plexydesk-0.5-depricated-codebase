#ifndef PLEXY_CLASSIC_BACKDROP_H
#define PLEXY_CLASSIC_BACKDROP_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <datainterface.h>
#include <pluginloader.h>
#include <abstractplugininterface.h>


class VISIBLE_SYM ClassicBackdrop :public PlexyDesk::BackdropInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
        ClassicBackdrop(QObject * object = 0);
        virtual ~ClassicBackdrop();
        void render(QPainter *p,QRectF r);
    public slots:
        void data(QVariant&);

    private:
        QPixmap bg;
        QImage img;
        QBrush paint;
        int width;
        int height;
};


#endif
