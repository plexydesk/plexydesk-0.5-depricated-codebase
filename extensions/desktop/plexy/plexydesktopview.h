#ifndef PLEXYDESKTOPVIEW_H
#define PLEXYDESKTOPVIEW_H

#include <QGraphicsView>

class PlexyDesktopView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent = 0);

    bool setBackgroundSource(const QString& source_id);

protected:
    virtual void dropEvent(QDropEvent *event);
    virtual void dragEnterEvent (QDragEnterEvent * event);
    virtual void dragMoveEvent (QDragMoveEvent * event);

private:
    class PrivatePlexyDesktopView;
    PrivatePlexyDesktopView *const d;

};

#endif // PLEXYDESKTOPVIEW_H
