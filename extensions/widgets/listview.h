#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <desktopwidget.h>
#include <plexyconfig.h>
#include <QAbstractListModel>
#include <QtGui> //TODO: remove

namespace PlexyDesk
{

class ListItem
{
public:
    ListItem() {
    }
    virtual ~ListItem() {
    }
    QString title;
    QString link;
    QString desc;
    QString thumb;
};

class PLEXYDESK_EXPORT ListView : public DesktopWidget
{
    Q_OBJECT

public:
    ListView(const QRectF &rect);
    virtual ~ListView();

    virtual void paintExtFace(QPainter *painter,
         const QStyleOptionGraphicsItem *e,
         QWidget *widget);
    virtual void paintExtDockFace(QPainter *painter,
         const QStyleOptionGraphicsItem *item,
         QWidget *widget);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
    virtual void insert(ListItem *item);

public Q_SLOTS:
    void clear();

private:
    void drawWidget();;
    class Private;
    Private *const d;
};

}
#endif // LISTVIEW_H
