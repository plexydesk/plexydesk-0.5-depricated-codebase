#include "facebookcontactcardbutton.h"
#include <QStyleOptionGraphicsItem>
#include <style.h>

FacebookContactCardButton::FacebookContactCardButton(QGraphicsObject *parent) :
    PlexyDesk::Button(parent)
{
}

void FacebookContactCardButton::paintNormalButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    PlexyDesk::StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = PlexyDesk::StyleFeatures::SF_None;
    feature.fontColor = QColor(255, 255, 255);
    feature.fontPen = QPen(QColor(255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    feature.fontFlags = Qt::AlignCenter | Qt::AlignVCenter;
    feature.font = QFont ("Georgia", 12, QFont::Bold);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    linearGrad.setColorAt(1, QColor(71, 160, 0));
    linearGrad.setColorAt(0, QColor(160, 214, 61));

    QLinearGradient borderGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    borderGrad.setColorAt(0, QColor(183, 229, 83));
    borderGrad.setColorAt(1, QColor(90, 170, 29));

    feature.buttonBorderGradient = borderGrad;

    feature.buttonGradiantMouseOver = linearGrad;

    if(style()) {
        style()->paintControlElement(PlexyDesk::Style::CE_PushButton, feature, painter);
        style()->paintControlElementText(PlexyDesk::Style::CE_PushButton, feature, label(), painter);
    }
}

void FacebookContactCardButton::paintSunkenButton(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    PlexyDesk::StyleFeatures feature;
    feature.exposeRect = option->exposedRect;
    feature.state = PlexyDesk::StyleFeatures::SF_Raised;
    feature.fontColor = QColor(255, 255, 255);
    feature.fontPen = QPen (QColor(255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    feature.fontFlags = Qt::AlignCenter | Qt::AlignVCenter;
    feature.font = QFont ("Georgia", 12, QFont::Bold);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    linearGrad.setColorAt(0, QColor(71, 160, 0));
    linearGrad.setColorAt(1, QColor(160, 214, 61));

    QLinearGradient borderGrad(QPointF(0, 0), QPointF(0.0 , option->exposedRect.height()));
    borderGrad.setColorAt(0, QColor(183, 229, 83));
    borderGrad.setColorAt(1, QColor(90, 170, 29));

    feature.buttonBorderGradient = borderGrad;
    feature.buttonGradiantRaised = linearGrad;

    if(style()) {
        style()->paintControlElement(PlexyDesk::Style::CE_PushButton, feature, painter);
        style()->paintControlElementText(PlexyDesk::Style::CE_PushButton, feature, label(), painter);
    }
}
