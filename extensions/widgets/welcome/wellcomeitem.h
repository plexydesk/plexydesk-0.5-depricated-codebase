//GPL2 siraj@kde.org


#ifndef WELLCOME_ITEM_H
#define WELLCOME_ITEM_H

#include <config.h>
#include <plexy.h>
#include <desktopwidget.h>
#include <QtGui>
#include <QtCore>
#include <qimageblitz.h>


class  WellcomeItem:public QObject, public QGraphicsRectItem
{
Q_OBJECT
    public:

    typedef enum { REGULAR,OVER,PRESSED } MouseState;
    typedef QHash <MouseState,QString> ThemeNames;

    WellcomeItem(const QRectF &rect,QGraphicsItem * parent = 0);
    virtual ~WellcomeItem();


    void  paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
   // void  paintExtDockFace( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0){}

    
    bool isEmpty() const {return false;}
    
    QString name() const ;
    void setName(const QString& name);
    
    void setOpacity(float op);

    void setIcon(const QPixmap &icon);

    public slots:
        void zoom(int step);
    signals:
        void  clicked();
    protected:
	virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
	virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    protected:
        QImage reflection(QImage& src);
        void paintItem(QPainter * painter,const QRectF );
    //    void paintItemRef(QPainter * painter,const QRectF);
    private:
    QString loadSvg(MouseState state);
    class Private;
    Private * const d ;

};

#endif
