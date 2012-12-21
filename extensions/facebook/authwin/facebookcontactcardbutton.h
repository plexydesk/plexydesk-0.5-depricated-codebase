#ifndef FACEBOOKCONTACTCARDBUTTON_H
#define FACEBOOKCONTACTCARDBUTTON_H

#include <button.h>

class FacebookContactCardButton : public PlexyDesk::Button
{
    Q_OBJECT
public:
    explicit FacebookContactCardButton(QGraphicsObject *parent = 0);

protected:
    void paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void paintSunkenButton(QPainter *painter, const QStyleOptionGraphicsItem *option);
};

#endif // FACEBOOKCONTACTCARDBUTTON_H
