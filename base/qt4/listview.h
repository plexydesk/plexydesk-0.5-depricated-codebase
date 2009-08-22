#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QtGui> //todo: remove
#include <plexyconfig.h>
#include <desktopwidget.h>
#include <QAbstractListModel>

namespace PlexyDesk
{

class ListItem {

public:
        ListItem(){}
        virtual ~ListItem(){}
        QString title;
        QString link;
        QString desc;
        QString thumb;
};

class VISIBLE_SYM ListView : public DesktopWidget
{
    Q_OBJECT

public:
    ListView(const QRectF &rect, QWidget *embeddedWidget = 0);
    virtual ~ListView();
    virtual void paintExtFace(QPainter* painter, const QStyleOptionGraphicsItem*e, QWidget*);
    virtual void paintExtDockFace(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    virtual void wheelEvent(QGraphicsSceneWheelEvent* event);

private:
  //  void setPath(QString);
    void drawWidget();;
    class Private;
    Private * const d;
};

}
#endif // LISTVIEW_H
