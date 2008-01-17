#ifndef PLEXY_WIDGET_FRIENDS_H
#define PLEXY_WIDGET_FRIENDS_H

#include <QtCore>
#include <plexy.h>
#include <widgetinterface.h>


class VISIBLE_SYM Friends : public QObject , public PlexyDesk::WidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::WidgetInterface)

    public:
    Friends(QObject * object = 0);
    virtual ~Friends();
    virtual QGraphicsItem * backdrop();// {};
};


#endif
