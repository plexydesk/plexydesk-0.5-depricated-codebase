#ifndef PLEXYDESKTOPVIEW_H
#define PLEXYDESKTOPVIEW_H

#include <QGraphicsView>
#include <abstractdesktopview.h>

class PlexyDesktopView : public PlexyDesk::AbstractDesktopView
{
    Q_OBJECT
public:
    explicit PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent = 0);

protected:
    virtual void dropEvent(QDropEvent *event);
    virtual void dragEnterEvent (QDragEnterEvent *event);
    virtual void dragMoveEvent (QDragMoveEvent *event);

private:
    class PrivatePlexyDesktopView;
    PrivatePlexyDesktopView *const d;

};

#endif // PLEXYDESKTOPVIEW_H
