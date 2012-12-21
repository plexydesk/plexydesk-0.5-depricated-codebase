#include "button.h"

#include <svgprovider.h>
#include <nativestyle.h>

namespace PlexyDesk {

class Button::PrivateButton
{
public:
    typedef enum {
        NORMAL,
        PRESS,
        HOVER
    } ButtonState;

    PrivateButton() {}
    ~PrivateButton() {
    }

    ButtonState mState;
    Style *mStyle;
    QString mLabel;
    QSize mSize;
};

Button::Button(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d(new PrivateButton)
{
   d->mState = PrivateButton::NORMAL;
   d->mSize = QSize (100, 30);
   d->mStyle = new NativeStyle(this);
}

Button::~Button()
{
    delete d;
}

void Button::setLabel(const QString &txt)
{
    d->mLabel = txt;
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /*widget*/)
{
    switch(d->mState) {
       case PrivateButton::NORMAL: paintNormalButton(painter, option) ; break;
       case PrivateButton::PRESS: paintSunkenButton(painter, option) ; break;
    default:
        qDebug() << Q_FUNC_INFO << "Unknown Button State";
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    d->mState = PrivateButton::PRESS;
    update();
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    d->mState = PrivateButton::NORMAL;
    update();
    Q_EMIT clicked();
}

void Button::paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_None;
    feature.fontColor = QColor (255, 255, 255);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    linearGrad.setColorAt(1, QColor(35, 112, 189));
    linearGrad.setColorAt(0, QColor(50, 139, 222));

    QLinearGradient borderGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    borderGrad.setColorAt(0, QColor(98, 101, 108));
    borderGrad.setColorAt(1, QColor(98, 101, 108));

    feature.buttonGradiantMouseOver = linearGrad;
    feature.buttonBorderGradient = borderGrad;

    if(d->mStyle) {
        d->mStyle->paintControlElement(Style::CE_PushButton, feature, painter);
        d->mStyle->paintControlElementText(Style::CE_PushButton, feature, d->mLabel, painter);
    }
}

void Button::paintSunkenButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_Raised;
    feature.fontColor = QColor (255, 255, 255);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    linearGrad.setColorAt(0, QColor(35, 112, 189));
    linearGrad.setColorAt(1, QColor(50, 139, 222));

    QLinearGradient borderGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    borderGrad.setColorAt(0, QColor(220, 220, 220));
//    linearGrad.setColorAt(1, QColor(50, 139, 222));

    feature.buttonGradiantRaised = linearGrad;
    feature.buttonBorderGradient = borderGrad;

    if(d->mStyle) {
        d->mStyle->paintControlElement(Style::CE_PushButton, feature, painter);
        d->mStyle->paintControlElementText(Style::CE_PushButton, feature, d->mLabel, painter);
    }
}

//tmp until we make styles a plugin type.
void Button::setStyle(Style *style)
{
    d->mStyle = style;
}

Style *Button::style()
{
    return d->mStyle;
}

QRectF Button::boundingRect() const
{
    return QRectF(0.0, 0.0, d->mSize.width(), d->mSize.height());
}

void Button::setSize(const QSize &size)
{
    d->mSize = size;
}

void Button::setIcon(const QImage & /*img*/)
{
}

QString Button::label() const
{
    return d->mLabel;
}

}
