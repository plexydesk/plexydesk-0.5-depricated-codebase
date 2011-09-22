#ifndef PLEXYBASEUI_H
#define PLEXYBASEUI_H

#include <QWidget>

class DesktopBaseUi : public QObject
{
    Q_OBJECT
public:
    DesktopBaseUi(QObject *parent = 0);
    virtual ~DesktopBaseUi();

    QRect desktopRect() const;
public Q_SLOTS:
    void screenResized(int screen);

private:
    void setup();
    class DesktopBaseUiPrivate;
    DesktopBaseUiPrivate *const d;
};

#endif // PLEXYBASEUI_H
