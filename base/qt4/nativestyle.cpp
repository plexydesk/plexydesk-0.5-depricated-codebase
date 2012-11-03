#include "nativestyle.h"
#include <QDebug>

namespace PlexyDesk {
NativeStyle::NativeStyle(QObject *parent) :
    Style(parent)
{
}

void NativeStyle::paintControlElement(Style::ControlElement element, const StyleFeatures &feature, QPainter *painter)
{
    switch(element) {
    case CE_PushButton : drawPushButton(feature, painter); break;
    case CE_Frame : drawFrame(feature, painter); break;
    case CE_LineEdit: drawLineEdit(feature, painter); break;
    default:
        qWarning() << Q_FUNC_INFO << "Unknown Control Element";
    }
}

void NativeStyle::paintControlElementText(Style::ControlElement element, const StyleFeatures &feature, const QString &text, QPainter *painter)
{
    switch(element) {
    case CE_PushButton : drawPushButtonText(feature, text, painter); break;
    default:
        qWarning() << Q_FUNC_INFO << "Unknown Control Element";
    }

}

void NativeStyle::drawPushButton(const StyleFeatures &features, QPainter *painter)
{
    QRectF rect = features.exposeRect;

    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect, 3.5, 3.5);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , rect.height()));

    if(features.state == StyleFeatures::SF_Raised) {
        linearGrad.setColorAt(0, QColor(35, 112, 189));
        linearGrad.setColorAt(1, QColor(50, 139, 222));
        //QColor(189, 191, 196)
        painter->fillPath(backgroundPath, linearGrad);
        QPen pen(QColor(56, 86, 122), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(backgroundPath);
    }
    else {
        linearGrad.setColorAt(1, QColor(35, 112, 189));
        linearGrad.setColorAt(0, QColor(50, 139, 222));
        //QColor(189, 191, 196)
        painter->fillPath(backgroundPath, linearGrad);
        QPen pen(QColor(56, 86, 122), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(backgroundPath);
    }
}

void NativeStyle::drawFrame(const StyleFeatures &features, QPainter *painter)
{
    QRectF rect = features.exposeRect;

    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect, 4, 4);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , rect.height()));

    linearGrad.setColorAt(1, QColor(189, 191, 196));
    linearGrad.setColorAt(0, QColor(255, 255, 255));
    //QColor(189, 191, 196)
    painter->fillPath(backgroundPath, linearGrad);
    QPen pen(QColor(98, 101, 108), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(backgroundPath);
}

void NativeStyle::drawPushButtonText(const StyleFeatures &features, const QString &text, QPainter *painter)
{
    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPen pen(QColor(255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(QColor(255, 255, 255));
    painter->drawText(features.exposeRect, Qt::AlignCenter, text);
}

void NativeStyle::drawLineEdit(const StyleFeatures &features, QPainter *painter)
{
    QRectF rect = features.exposeRect;

    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect, 2, 2);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0.0 , rect.height()));

    linearGrad.setColorAt(0, QColor(189, 191, 196));
    linearGrad.setColorAt(1, QColor(255, 255, 255));

    painter->fillPath(backgroundPath, linearGrad);
    QPen pen(QColor(98, 101, 108), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(backgroundPath);
}

}
