#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <plexy.h>
#include <QGraphicsScene>
#include <QGraphicsObject>

#include <style.h>

namespace PlexyDesk {

class Button : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Button(QGraphicsObject *parent = 0);

    virtual ~Button();

    virtual void setLable(const QString &txt);

    virtual QString label() const;

    virtual void setIcon(const QImage &img);

    virtual void setStyle(Style *style);

    virtual QRectF boundingRect() const;

Q_SIGNALS:
    void clicked();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    class PrivateButton;
    PrivateButton *const d;
};

}

#endif // UI_BUTTON_H
