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
        if (mRender)
            delete mRender;
    }

    void paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option);

    void paintSunkenButton(QPainter *painter, const QStyleOptionGraphicsItem *option);

    ButtonState mState;
    SvgProvider *mRender;
    Style *mStyle;
    QString mLabel;

};

Button::Button(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d(new PrivateButton)
{
   d->mState = PrivateButton::NORMAL;
   d->mRender = new SvgProvider();
   d->mStyle = new NativeStyle(this);
}

Button::~Button()
{
    delete d;
}

void Button::setLable(const QString &txt)
{
    d->mLabel = txt;
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(d->mState) {
       case PrivateButton::NORMAL: d->paintNormalButton(painter, option) ; break;
       case PrivateButton::PRESS: d->paintSunkenButton(painter, option) ; break;
    default:
        qDebug() << Q_FUNC_INFO << "Unknown Button State";
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    d->mState = PrivateButton::PRESS;
    update();
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    d->mState = PrivateButton::NORMAL;
    update();
    Q_EMIT clicked();
}

void Button::PrivateButton::paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_None;

    if(mStyle) {
        mStyle->paintControlElement(Style::CE_PushButton, feature, painter);
        mStyle->paintControlElementText(Style::CE_PushButton, feature, mLabel, painter);
    }
}

void Button::PrivateButton::paintSunkenButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_Raised;

    if(mStyle) {
        mStyle->paintControlElement(Style::CE_PushButton, feature, painter);
        mStyle->paintControlElementText(Style::CE_PushButton, feature, mLabel, painter);
    }
}

//tmp until we make styles a plugin type.
void Button::setStyle(Style *style)
{
    d->mStyle = style;
}

QRectF Button::boundingRect() const
{
    return QRectF(0.0, 0.0, 100, 30);
}

void Button::setIcon(const QImage &img)
{
}

QString Button::label() const
{
}

}
