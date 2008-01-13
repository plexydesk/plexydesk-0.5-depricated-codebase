#ifndef PLEXY_DESKTOP_VIEW_H
#define PLEXY_DESKTOP_VIEW_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>


namespace PlexyDesk
{

class VISIBLE_SYM  DesktopView : public QGraphicsView
{
    Q_OBJECT
    public:
        DesktopView ( QGraphicsScene * scene = new QGraphicsScene(), QWidget * parent = 0 );
        virtual ~DesktopView();

        void addWallpaper(const QString& path);

        void addRender(QGraphicsScene * scene);

        void addDesktopWidget(QWidget * widget);

        void addDesktopItem (QGraphicsItem * item);

    protected:
        void drawBackground ( QPainter * painter, const QRectF & rect );
	//void paintEvent(QPaintEvent * event);

    private:
            class Private;
            Private * const d ;

};


} // namespace #ifndef
#endif
