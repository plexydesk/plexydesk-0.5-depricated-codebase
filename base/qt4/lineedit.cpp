#include "lineedit.h"

#include <style.h>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

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
    void paintFocusedEdit(QPainter *painter, const QStyleOptionGraphicsItem *option);

    EditState mState;
    Style *mStyle;
    QString mText;
    int mLastKey;
    int mKeyCursorLeftLoc;
    QSizeF mSize;
};

LineEdit::LineEdit(QGraphicsObject *parent) :
    QGraphicsObject(parent),
    d (new PrivateLineEdit)
{
    d->mState = PrivateLineEdit::NORMAL;
    d->mStyle = new NativeStyle(this);
    setAcceptHoverEvents(true);
    d->mKeyCursorLeftLoc = 0;
}

LineEdit::~LineEdit()
{
    delete d;
}

void LineEdit::setText(const QString &txt)
{
    d->mText = txt;
}

QString LineEdit::text() const
{
    return d->mText;
}

void LineEdit::setStyle(Style *style)
{
    d->mStyle = style;
}

QRectF LineEdit::boundingRect() const
{
    return QRectF(0.0, 0.0, d->mSize.width(), d->mSize.height());
}

void LineEdit::setSize(const QSizeF &size)
{
    d->mSize = size;
    update();
}

void LineEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch(d->mState) {
       case PrivateLineEdit::NORMAL: d->paintNormalEdit(painter, option) ; break;
       case PrivateLineEdit::FOCUSED: d->paintFocusedEdit(painter, option) ; break;
    default:
        qDebug() << Q_FUNC_INFO << "Unknown State";
    }
}

void LineEdit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    d->mState = PrivateLineEdit::FOCUSED    ;
    update();
}

void LineEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

void LineEdit::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::IBeamCursor);
    d->mState = PrivateLineEdit::FOCUSED;
    update();
}

void LineEdit::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::IBeamCursor);
    grabKeyboard();
    d->mState = PrivateLineEdit::FOCUSED;
    update();
}

void LineEdit::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    ungrabKeyboard();
    d->mState = PrivateLineEdit::NORMAL;
    setCursor(Qt::ArrowCursor);
    update();
}

void LineEdit::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::IBeamCursor);
    d->mState = PrivateLineEdit::FOCUSED;
    update();
}

void LineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace) {
        d->mText.remove(d->mKeyCursorLeftLoc - 1, 1);
        if (d->mKeyCursorLeftLoc != 0)
            d->mKeyCursorLeftLoc--;
        Q_EMIT text(d->mText);
        update();
        return;
    } else if (event->key() == Qt::Key_Left) {
        if (d->mKeyCursorLeftLoc != 0)
            d->mKeyCursorLeftLoc--;
        update();
        return;

    } else if (event->key() == Qt::Key_Right) {
        if (d->mKeyCursorLeftLoc < d->mText.count())
            d->mKeyCursorLeftLoc++;
        update();
        return;
    }
    d->mText.insert(d->mKeyCursorLeftLoc, event->text());
    d->mKeyCursorLeftLoc++;
    Q_EMIT text(d->mText);
    update();
}

void LineEdit::PrivateLineEdit::paintNormalEdit(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_None;
    feature.cursorLocation = mKeyCursorLeftLoc;

    if(mStyle) {
        mStyle->paintControlElement(Style::CE_LineEdit, feature, painter);
        mStyle->paintControlElementText(Style::CE_LineEdit, feature, mText, painter);
    }
}

void LineEdit::PrivateLineEdit::paintFocusedEdit(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = StyleFeatures::SF_MouseOver;
    feature.cursorLocation = mKeyCursorLeftLoc;

    if(mStyle) {
        mStyle->paintControlElement(Style::CE_LineEdit, feature, painter);
        mStyle->paintControlElementText(Style::CE_LineEdit, feature, mText, painter);
    }
}

}
