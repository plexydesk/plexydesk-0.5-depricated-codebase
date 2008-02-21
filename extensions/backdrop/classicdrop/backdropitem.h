#ifndef PLEXY_BACK_DROP_ITEM_CLASSIC_H
#define PLEXY_BACK_DROP_ITEM_CLASSIC_H

#include <QtCore>
#include <datainterface.h>
#include <pluginloader.h>
#include <QtGui>
#include <plexy.h>


namespace PlexyDesk
{

    class BackdropItem: public QObject, public QGraphicsRectItem
    {

    Q_OBJECT
        public:
        BackdropItem(const QRectF &rect);
        virtual ~BackdropItem();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QRectF boundingRect() const;
        public slots:
            void data(QVariant&);
        signals:
            void newWall(QVariant&);
        private:
        QPixmap bg;

    };


} // namespace PlexyDesk

#endif
