#include "listview.h"

#include <QImage>

namespace PlexyDesk
{
class ListView::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QRectF clip;
    QRectF view;
    QString prefix;
    QImage m_bg;
    QImage mItem_bg;
    QList<ListItem *> mItemList;
};

ListView::ListView(const QRectF &rect, QWidget *win) :
    DesktopWidget(rect, win), d(new Private)
{
    d->clip = QRectF(10, 10, rect.width() - 40, rect.height() - 20);
    d->view = QRectF(0, 0, rect.width() - 80, 0);
    d->prefix = applicationDirPath() + "/theme/skins/default/widget/default/youtube/";
    d->m_bg = QImage(d->prefix + "bacground.png");
    d->mItem_bg = QImage(d->prefix + "videotag.png");
}

ListView::~ListView()
{
    delete d;
}

void ListView::clear()
{
    d->mItemList.clear();
}

void ListView::insert(ListItem *item)
{
    d->mItemList.append(item);
    d->view.setHeight(d->mItemList.size()*d->mItem_bg.height());
}

void ListView::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *)
{
    QRectF r = e->exposedRect;
    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(rect(), Qt::transparent);
    p->drawImage(QRect(r.x(), r.y(), d->m_bg.width(), d->m_bg.height()), d->m_bg);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);
    p->save();
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setClipRect(d->clip);
    int vidcount = 0;
    for (int i = 5; i < d->mItemList.size()* d->mItem_bg.height(); ) {
        if (vidcount >= d->mItemList.size())
            break;
        ListItem *videoentity;
        videoentity = d->mItemList.at(vidcount++);
        p->setPen(QColor(255, 255, 255));
        p->setFont(QFont("Bitstream Charter", 10, QFont::Bold));
        p->drawText(40, d->view.y() + i + 15, 300, 30, Qt::AlignLeft, QString(videoentity->title));
        p->setFont(QFont("Bitstream Charter", 8));
        p->drawText(40, d->view.y() + i + 40, 250, 100, Qt::AlignLeft | Qt::TextWordWrap, QString(videoentity->desc));
        p->drawImage(QRect(d->clip.x() + 1, d->view.y() + i, d->mItem_bg.width(), d->mItem_bg.height()), d->mItem_bg);
        i += d->mItem_bg.height();
    }
    p->restore();
}

void ListView::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    update();
    int numDegrees = event->delta() / 2;
    int numSteps = numDegrees / 15;
    int expectedY = d->view.y();
    if (expectedY == 0 && numSteps > 0) {
        return;
    }
    if (abs(d->view.y()) > (d->view.bottom() - d->clip.height()) && numSteps < 0)
        return;
    d->view.setY(d->view.y() + numSteps);
}

void ListView::paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *e, QWidget *w)
{
    DesktopWidget::paintExtDockFace(painter, e, w);
}

void ListView::drawWidget()
{
}

}
