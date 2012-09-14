#include "iconwidgetview.h"

#include <QGraphicsProxyWidget>
#include <QListView>
#include <QFileSystemModel>
#include <QPropertyAnimation>

#include <desktopwidget.h>

class IconWidgetView::PrivateIconWidgetView
{
public:
    PrivateIconWidgetView() {}
    ~PrivateIconWidgetView() {
        delete mIconView;
        delete mInfoView;
        delete mSlideAnimation;
    }

    QGraphicsProxyWidget *mWidgetProxy;
    QListView *mIconView;
    QFileSystemModel *mFileSystemModel;
    PlexyDesk::DesktopWidget *mInfoView;
    QPropertyAnimation *mSlideAnimation;
};

IconWidgetView::IconWidgetView(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::ScrollWidget(rect, parent),
    d(new PrivateIconWidgetView)
{
    d->mWidgetProxy = new QGraphicsProxyWidget(this);
    d->mIconView = new QListView;

    d->mIconView->setViewMode(QListView::IconMode);
    d->mIconView->setIconSize(QSize(64, 64));
    d->mIconView->setUniformItemSizes(true);
    d->mIconView->setGridSize(QSize(96, 96));
    d->mIconView->setAcceptDrops(true);
    d->mIconView->setDropIndicatorShown(true);
    ///hack until we have our own painting
    d->mIconView->setStyleSheet(" border : 0 ; background-color: transparent; color : #ffffff");

    d->mWidgetProxy->setWidget(d->mIconView);

    QRectF iconViewRect = QRectF(12.0, 12.0, rect.width() - 24.0, rect.height() - 24.0);
    QRectF infoViewRect = QRectF(0.0,  0.0, rect.width(), 240);

    d->mInfoView = new PlexyDesk::DesktopWidget(infoViewRect, this);
    d->mInfoView->setPos(QPointF(0.0, rect.height()));
    QPointF infoViewPos = d->mInfoView->pos();

    d->mWidgetProxy->setPos(iconViewRect.x(), iconViewRect.y());
    d->mWidgetProxy->resize(iconViewRect.size());

    d->mFileSystemModel = new QFileSystemModel(d->mIconView);
    d->mIconView->setModel(d->mFileSystemModel);

    connect(d->mFileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT (onDirLoaded(const QString)));
    connect(d->mIconView, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));
    connect(d->mInfoView, SIGNAL(clicked()), this, SLOT(infoViewClicked()));

    d->mInfoView->enableDefaultBackground(true);
    d->mInfoView->show();

    //slide animation for info view
    d->mSlideAnimation = new QPropertyAnimation(d->mInfoView, "pos");
    d->mSlideAnimation->setDuration(1000);
    d->mSlideAnimation->setStartValue(infoViewPos);
    d->mSlideAnimation->setEndValue(QPointF(0.0, 140.0));
    d->mSlideAnimation->setEasingCurve(QEasingCurve::InCirc);

    d->mWidgetProxy->show();
}

IconWidgetView::~IconWidgetView()
{
    delete d;
}

void IconWidgetView::setDirectoryPath(const QString &path)
{
    d->mFileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    d->mFileSystemModel->setRootPath(path);
    d->mIconView->setRootIndex(d->mFileSystemModel->index(path));
}

void IconWidgetView::onDirLoaded(const QString &path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void IconWidgetView::onClicked(const QModelIndex &index)
{
    d->mSlideAnimation->setDirection(QAbstractAnimation::Forward);
    d->mSlideAnimation->start();
}

void IconWidgetView::infoViewClicked()
{
    d->mSlideAnimation->setDirection(QAbstractAnimation::Backward);
    d->mSlideAnimation->start();
}
