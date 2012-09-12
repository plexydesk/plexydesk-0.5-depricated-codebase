#include "iconwidgetview.h"

#include <QGraphicsProxyWidget>
#include <QListView>

class IconWidgetView::PrivateIconWidgetView
{
public:
    PrivateIconWidgetView() {}
    ~PrivateIconWidgetView() {}

    QGraphicsProxyWidget *mWidgetProxy;
    QListView *mIconView;

};

IconWidgetView::IconWidgetView(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::ScrollWidget(rect, parent),
    d(new PrivateIconWidgetView)
{
    d->mWidgetProxy = new QGraphicsProxyWidget(this);
    d->mIconView = new QListView;
    d->mWidgetProxy->setWidget(d->mIconView);
    d->mWidgetProxy->show();
}

void IconWidgetView::setDirectoryPath(const QString &path)
{
}
