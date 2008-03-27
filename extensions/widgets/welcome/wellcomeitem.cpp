#include "wellcomeitem.h"
#include <QtCore>
#include "wellcomeitem.moc"

class
    WellcomeItem::Private
{
public:
    Private ()
    {
    }
    ~
        Private ()
    {
    }
    int height;
    int  width;
    QString text;
    QString comment;
    QString tooltip;
    QString icon;
    int Rating;
    MouseState state;
    ThemeNames ids;
    QSize size;
    QSize elementSize;
    QPixmap iconPixmap;
    float opacity;
    //Grid
    int ncol;
    int nrow;
    QTimeLine timeline;
    QImage refimg;
    bool blured;
};


WellcomeItem::WellcomeItem (const QRectF &rect, QGraphicsItem * parent ):QGraphicsRectItem(rect,parent),
d (new Private)
{
    //TODO
    d->height = 128;
    d->width =  128;
    d->size = QSize(d->width,d->height);
    d->iconPixmap = QPixmap ();
    d->opacity = 1.0f;
    d->timeline.setDuration (200);
    d->timeline.setFrameRange (0, 120);
    d->timeline.setCurveShape(QTimeLine::EaseInCurve);
    connect (&d->timeline, SIGNAL (frameChanged (int)), this,
        SLOT (zoom (int)));
    d->blured = false;
    d->refimg = QImage (d->size, QImage::Format_ARGB32_Premultiplied);
    d->refimg = reflection (d->refimg);
    setAcceptsHoverEvents (true);

}

WellcomeItem::~WellcomeItem ()
{
    delete d;
}

QImage WellcomeItem::reflection (QImage & img)
{
    QImage reflect = img.mirrored (0, 1);
    QPainter p (&reflect);
    QPoint x, y;

    y.setY (reflect.height ());
    QLinearGradient grad (x, y);
    grad.setColorAt (0, QColor (0, 0, 0,95));
    grad.setColorAt (1, Qt::transparent);
    p.setCompositionMode (QPainter::CompositionMode_DestinationIn);

    p.fillRect (QRect (0, 0, reflect.width (), reflect.height ()), grad);
    p.end ();
    return reflect;
}

void WellcomeItem::paintItem (QPainter * painter,
                         const QRectF rect)
{
    painter->setRenderHint (QPainter::SmoothPixmapTransform,true);

    QRectF source (0, 0, d->width,d->height);
    if (!d->iconPixmap.isNull ())
    {
        painter->
            drawPixmap (int
            (((int) source.width () - d->iconPixmap.width ()) / 2),
            int (((int) source.height () -
            d->iconPixmap.height ()) / 2),
            d->iconPixmap.height (), d->iconPixmap.width (),
            d->iconPixmap);
    }
}
void WellcomeItem::paint (QPainter * painter,
                     const QStyleOptionGraphicsItem * option,
                     QWidget * widget)
{
    Q_UNUSED (widget);
    painter->save();
    painter->setClipRect(option->exposedRect);
    // painter->setCompositionMode(QPainter::CompositionMode_Source);
    //  painter->fillRect(option->exposedRect, Qt::transparent);
    //  painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    this->paintItem (painter, option->exposedRect);
    painter->drawImage(QPoint( (d->width -  d->iconPixmap.width())/2 ,d->iconPixmap.height()) ,d->refimg);
    painter->restore();
}

void WellcomeItem::setOpacity (float op)
{
    d->opacity = op;
}

QString WellcomeItem::loadSvg (MouseState state)
{
    return d->ids[state];
}

//Events

void WellcomeItem::hoverEnterEvent (QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED (event);
    d->state = OVER;
    update ();
    d->timeline.setDirection (QTimeLine::Forward);
    if (d->timeline.state () == QTimeLine::NotRunning)
        d->timeline.start ();
    emit clicked ();
}

void WellcomeItem::hoverMoveEvent (QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED (event);
    d->state = OVER;
    //update ();

}

void WellcomeItem::hoverLeaveEvent (QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED (event);
    d->state = REGULAR;
    update ();
    d->timeline.setDirection (QTimeLine::Backward);
    if (d->timeline.state () == QTimeLine::NotRunning)
        d->timeline.start ();

}

QString WellcomeItem::name () const
{
    return d->text;
}

void WellcomeItem::setName (const QString & name)
{
    d->text = name;
}

void WellcomeItem::setIcon (const QPixmap &icon)
{
    d->iconPixmap = QPixmap (icon);
    //FIXME
    //Optimize
    QImage Buffer = icon.toImage();
    d->refimg = d->refimg.scaled(boundingRect().width(),boundingRect().height());
    QPainter p (&d->refimg);

    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setCompositionMode (QPainter::CompositionMode_Source);
    p.fillRect (QRect(0,0,boundingRect().width(),boundingRect().height()), Qt::transparent);

    Buffer = reflection(Buffer);
    Buffer = Blitz::blur (Buffer, 8);
    QTransform trans;
    //trans.translate(30,100);
    p.setTransform(trans);
    p.drawImage (QPoint (0,0),Buffer);
    p.end();

}

void WellcomeItem::zoom (int step)
{

    QPointF center = this->boundingRect ().center ();
    resetMatrix ();
    QTransform mat = this->transform ();
    mat.translate (center.x (), center.y ());
    mat.scale (1 + step / 350.0, 1 + step / 350.00);
    mat.translate (-(center.x ()), -center.y ());

    this->setTransform (mat);

}
