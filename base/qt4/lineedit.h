#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <plexy.h>
#include <style.h>

#include <QGraphicsObject>

namespace PlexyDesk {
class LineEdit : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit LineEdit(QGraphicsObject *parent = 0);

    virtual ~LineEdit();

    virtual void setText(const QString &txt);

    virtual QString text() const;

    virtual void setStyle(Style *style);

    virtual QRectF boundingRect() const;

    virtual void setSize (const QSizeF &size);

Q_SIGNALS:
    void clicked();
    void text(const QString &text);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    class PrivateLineEdit;
    PrivateLineEdit *const d;

};

}

#endif // LINEEDIT_H
