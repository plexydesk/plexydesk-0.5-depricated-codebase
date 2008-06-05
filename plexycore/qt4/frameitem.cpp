#include "frameitem.h"

void Frame::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

    render.render(painter,rect());
    /*
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing,false);
        QRectF r = boundingRect();
        painter->save();
        painter->translate (0.5,0.5);
      //  painter->fillRect(QRectF(6,6,r.width()-14,r.height()-14)  ,QColor(0,0,0));
        painter->restore ();
        //painter->translate(0.5,0.5);
        QPixmap top (QSize(render.boundsOnElement("g3329").width(), render.boundsOnElement("g3329").height() ));
        top.fill(Qt::transparent);
        QPainter p;
        p.begin(&top);
        p.save();
        p.translate(0.5,0.5);
        render.render(&p,"g3329");
        p.restore();
        p.end();
        painter->drawPixmap(boundingRect().x(),boundingRect().y(),top);

        QPixmap tileTop(QSize (boundingRect().width()+ 118,7));
        tileTop.fill(Qt::transparent);
        p.begin(&tileTop);
        p.save();
        p.translate(-(140*4)/7,0.5);
        render.render(&p,"g3341");
        p.restore();
        p.end();
        painter->drawPixmap(boundingRect().x()+7,boundingRect().y(),tileTop);

        QPixmap left (QSize(render.boundsOnElement("g3335").width(), render.boundsOnElement("g3335").height()));
        left.fill(Qt::transparent);
        p.begin(&left);
        p.save();
        p.translate(0.5,0.5);
        render.render(&p,"g3335");
        p.restore ();
        p.end();
        painter->drawPixmap(rect().width()-7,0,left);

        QPixmap leftSide (QSize (7,rect().height()+20));
        leftSide.fill(Qt::transparent);
        p.begin(&leftSide);
        p.save();
        p.translate(0.5,0.5);
        render.render(&p,"g3354");
        p.restore();
        p.end();
        painter->drawPixmap(0,-23,leftSide);

        QPixmap rightSide (QSize (7,rect().height()+20));
        rightSide.fill(Qt::transparent);
        p.begin(&rightSide);
        p.save();
        p.translate(0.5,0.5);
        render.render(&p,"right");
        p.restore ();
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
        painter->restore();
    */

}


