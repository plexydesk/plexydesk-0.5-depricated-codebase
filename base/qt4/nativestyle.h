#ifndef NATIVESTYLE_H
#define NATIVESTYLE_H

#include <style.h>

namespace PlexyDesk {

class NativeStyle : public Style
{
    Q_OBJECT
public:
    explicit NativeStyle(QObject *parent = 0);

    virtual void paintControlElement(ControlElement element, const StyleFeatures &feature, QPainter *painter);
    virtual void paintControlElementText(ControlElement element, const StyleFeatures &feature, const QString &text, QPainter *painter);

private:
    void drawPushButton(const StyleFeatures &features, QPainter *painter);
    void drawFrame(const StyleFeatures &features, QPainter *painter);
    void drawPushButtonText(const StyleFeatures &features, const QString &text, QPainter *painter);
    void drawLineEdit(const StyleFeatures &features, QPainter *painter);
    void drawLineEditText(const StyleFeatures &features, const QString &text, QPainter *painter);
    void drawSeperatorLine(const StyleFeatures &features, QPainter *painter);
};
}

#endif // NATIVESTYLE_H
