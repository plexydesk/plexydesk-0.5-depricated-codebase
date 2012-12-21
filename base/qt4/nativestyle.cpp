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
    case CE_Seperator: drawSeperatorLine(feature, painter); break;
    default:
        qWarning() << Q_FUNC_INFO << "Unknown Control Element";
    }
}

void NativeStyle::paintControlElementText(Style::ControlElement element, const StyleFeatures &feature, const QString &text, QPainter *painter)
{
    switch(element) {
    case CE_PushButton : drawPushButtonText(feature, text, painter); break;
    case CE_LineEdit: drawLineEditText(feature, text, painter); break;
    case CE_Label: drawLabelEditText(feature, text, painter); break;
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

    if(features.state == StyleFeatures::SF_Raised) {
        painter->fillPath(backgroundPath, features.buttonGradiantRaised);
        QPen pen(features.buttonBorderGradient, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawPath(backgroundPath);
    }
    else {
        painter->fillPath(backgroundPath, features.buttonGradiantMouseOver);
        QPen pen(features.buttonBorderGradient, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
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
    QPen pen(QColor(98, 101, 108), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawPath(backgroundPath);
}

void NativeStyle::drawPushButtonText(const StyleFeatures &features, const QString &text, QPainter *painter)
{
    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPen pen;

    pen = QPen(QColor (111, 111, 111), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawText(features.exposeRect.adjusted(-1, -1, -1, -1), Qt::AlignCenter, text);

    pen = QPen(features.fontColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
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
    QPen pen;
    if(features.state == StyleFeatures::SF_MouseOver) {
        pen = QPen(QColor(98, 101, 208), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    } else {
        pen = QPen(QColor(98, 101, 108), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    painter->setPen(pen);
    painter->drawPath(backgroundPath);
}

void NativeStyle::drawLineEditText(const StyleFeatures &features, const QString &text, QPainter *painter)
{
    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    QPen pen;// () pen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    pen = QPen(QColor(255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QFont font = QFont("", 18);
    QFontMetrics fontMetrics(font);
    int width = fontMetrics.width(text.left(features.cursorLocation));

    painter->setFont(font);
    painter->setPen(pen);
    painter->drawText(features.exposeRect.adjusted(11.0, 1.0, 1.0, 1.0), Qt::AlignLeft | Qt::AlignVCenter, text);

    pen = QPen(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawText(features.exposeRect.adjusted(10.0, 0.0, 0.0, 0.0), Qt::AlignLeft | Qt::AlignVCenter, text);

    pen = QPen(QColor(98, 101, 108), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawLine(QPoint(width + 11, 4), QPoint(width + 11,features.exposeRect.height() - 4));
}

void NativeStyle::drawLabelEditText(const StyleFeatures &features, const QString &text, QPainter *painter)
{
    QPen pen;

    /* Painter settings */
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    painter->setFont(features.font);

    pen = QPen(QColor (255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawText(features.exposeRect.adjusted(1, 1, 1, 1), features.fontFlags, text);

    painter->setOpacity(0.8);

    pen = QPen(features.fontColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawText(features.exposeRect, features.fontFlags, text);
}


void NativeStyle::drawSeperatorLine(const StyleFeatures &features, QPainter *painter)
{
    QPen pen = QPen(QColor(217, 217, 217), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawLine(QPoint(features.exposeRect.x(), features.exposeRect.y()),
                      QPoint(features.exposeRect.width(), features.exposeRect.height()));
    pen = QPen(QColor(255, 255, 255), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawLine(QPoint(features.exposeRect.x() + 1, features.exposeRect.y() + 1),
                      QPoint(features.exposeRect.width() + 1, features.exposeRect.height() + 1));
}

}
