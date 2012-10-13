#include "button.h"

#include <svgprovider.h>

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

    ButtonState mState;
    SvgProvider *mRender;

};

Button::Button(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d(new PrivateButton)
{
   d->mState = PrivateButton::NORMAL;
   d->mRender = new SvgProvider();
}

Button::~Button()
{
    delete d;
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(d->mState) {
    case PrivateButton::NORMAL: d->paintNormalButton(painter, option) ; break;
    default:
        qDebug() << Q_FUNC_INFO << "Unknown Button State";
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    d->mState = PrivateButton::PRESS;
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    d->mState = PrivateButton::NORMAL;
}

void Button::PrivateButton::paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
}

}
