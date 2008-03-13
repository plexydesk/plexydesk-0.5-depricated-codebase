#include "frameitem.h"

void Frame::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->fillRect(QRectF(6,6,rect().width()-12,rect().height()-38),QColor(0,0,0));

    QPixmap top (QSize(7,7));
            top.fill(Qt::transparent);
    QPainter p;
    p.begin(&top);
    render.render(&p,"g3329");
    p.end();
    painter->drawPixmap(0,0,top);
    
    QPixmap tileTop(QSize (6,7));
    tileTop.fill(Qt::transparent);
    p.begin(&tileTop);
    render.render(&p,"g3341",QRectF(-1.0,0.0,8.0,7.0));
    p.end();
    painter->drawTiledPixmap(QRectF(6,0,rect().width()-12,7),tileTop);

    QPixmap left (QSize (7,7));
    left.fill(Qt::transparent);
    p.begin(&left);
    render.render(&p,"g3335");
    p.end();
    painter->drawPixmap(rect().width()-7,0,left);

    QPixmap leftSide (QSize (7,rect().height()+20));
    leftSide.fill(Qt::transparent);
    p.begin(&leftSide);
    render.render(&p,"g3354");
    p.end();
    painter->drawPixmap(0,-23,leftSide);
    
    QPixmap rightSide (QSize (7,rect().height()+20 ));
    rightSide.fill(Qt::transparent);
    p.begin(&rightSide);
    render.render(&p,"right");
    p.end();
    painter->drawPixmap(rect().width()-7,-23,rightSide);
    
    QPixmap botLeft (QSize (7,7));
    botLeft.fill(Qt::transparent);
    p.begin(&botLeft);
    render.render(&p,"g3350");
    p.end();
    painter->drawPixmap(0,rect().height()-33,botLeft);

    QPixmap botRight (QSize (7,7));
    botRight.fill(Qt::transparent);
    p.begin(&botRight);
    render.render(&p,"g3346");
    p.end();
    painter->drawPixmap(rect().width()-7,rect().height()-33,botRight);

    QPixmap tileBot(QSize (6,7));
    tileBot.fill(Qt::transparent);
    p.begin(&tileBot);
    render.render(&p,"g3359",QRectF(-1.0,0.0,8.0,7.0));
    p.end();
    painter->drawTiledPixmap(QRectF(6,rect().height()-33,rect().width()-12,7),tileBot);



}


