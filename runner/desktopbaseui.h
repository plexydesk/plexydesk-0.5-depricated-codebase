#ifndef PLEXYBASEUI_H
#define PLEXYBASEUI_H

#include <QWidget>
#include <QGraphicsView>

class DesktopBaseUi : public QWidget
{
    Q_OBJECT
public:
    DesktopBaseUi(QWidget *parent = 0);
    virtual ~DesktopBaseUi();

    void setDesktopView (const QString &name);

    QRect desktopRect() const;

    QList<QGraphicsView*> views() const;

public Q_SLOTS:
    void screenResized(int screen);

private:
    void setup();
    class DesktopBaseUiPrivate;
    DesktopBaseUiPrivate *const d;
};

#endif // PLEXYBASEUI_H
