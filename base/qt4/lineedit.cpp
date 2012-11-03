#include "lineedit.h"

#include <style.h>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

#include <nativestyle.h>

namespace PlexyDesk {

class LineEdit::PrivateLineEdit {
public:
    typedef enum {
        NORMAL,
        FOCUSED,
        HOVER
    } EditState;

    PrivateLineEdit() {}
    ~PrivateLineEdit() {}

    void paintNormalEdit(QPainter *painter, const QStyleOptionGraphicsItem *option);

    EditState mState;
    Style *mStyle;
};

LineEdit::LineEdit(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d (new PrivateLineEdit)
{
    d->mState = PrivateLineEdit::NORMAL;
    d->mStyle = new NativeStyle(this);
}

LineEdit::~LineEdit()
{
}

void LineEdit::setText(const QString &txt)
{
}

QString LineEdit::text() const
{
}

void LineEdit::setStyle(Style *style)
{
    d->mStyle = style;
}

QRectF LineEdit::boundingRect() const
{
    return QRectF(0.0, 0.0, 362, 30);
}

void LineEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(d->mState) {
       case PrivateLineEdit::NORMAL: d->paintNormalEdit(painter, option) ; break;
    default:
        qDebug() << Q_FUNC_INFO << "Unknown Button State";
    }
}

void LineEdit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void LineEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

void LineEdit::PrivateLineEdit::paintNormalEdit(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_None;

    if(mStyle) {
        mStyle->paintControlElement(Style::CE_LineEdit, feature, painter);
        //mStyle->paintControlElementText(Style::CE_LineEdit, feature, mLabel, painter);
    }
}

}
