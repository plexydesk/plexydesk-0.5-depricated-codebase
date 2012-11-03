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

Q_SIGNALS:
    void clicked();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    class PrivateLineEdit;
    PrivateLineEdit *const d;
    
};

}

#endif // LINEEDIT_H
