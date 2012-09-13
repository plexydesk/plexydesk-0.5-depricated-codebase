#include "iconwidgetview.h"

#include <QGraphicsProxyWidget>
#include <QListView>
#include <QFileSystemModel>

class IconWidgetView::PrivateIconWidgetView
{
public:
    PrivateIconWidgetView() {}
    ~PrivateIconWidgetView() {}

    QGraphicsProxyWidget *mWidgetProxy;
    QListView *mIconView;
    QFileSystemModel *mFileSystemModel;

};

IconWidgetView::IconWidgetView(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::ScrollWidget(rect, parent),
    d(new PrivateIconWidgetView)
{
    d->mWidgetProxy = new QGraphicsProxyWidget(this);
    d->mIconView = new QListView;
    d->mWidgetProxy->setWidget(d->mIconView);

    QRectF iconViewRect = QRectF(12.0, 12.0, rect.width() - 24.0, rect.height() - 24.0);

    d->mWidgetProxy->setPos(iconViewRect.x(), iconViewRect.y());
    d->mWidgetProxy->resize(iconViewRect.size());

    d->mWidgetProxy->show();

    d->mFileSystemModel = new QFileSystemModel(d->mIconView);
    d->mIconView->setModel(d->mFileSystemModel);

    setDirectoryPath("/home/siraj/");
}

void IconWidgetView::setDirectoryPath(const QString &path)
{
    d->mFileSystemModel->setFilter(QDir::AllDirs);
    d->mFileSystemModel->setRootPath(path);
}
