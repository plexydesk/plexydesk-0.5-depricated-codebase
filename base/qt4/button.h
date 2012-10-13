#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <plexy.h>
#include <QGraphicsScene>
#include <QGraphicsObject>

namespace PlexyDesk {

class Button : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Button(QGraphicsObject *parent = 0);

    ~Button();

    void setLable(const QString &txt);

    QString label() const;

    void setIcon(const QImage &img);


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
