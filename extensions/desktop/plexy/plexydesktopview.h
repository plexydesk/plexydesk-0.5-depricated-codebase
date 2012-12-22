#ifndef PLEXYDESKTOPVIEW_H
#define PLEXYDESKTOPVIEW_H

#include <QGraphicsView>
#include <abstractdesktopview.h>

class PlexyDesktopView : public PlexyDesk::AbstractDesktopView
{
    Q_OBJECT
public:
    explicit PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent = 0);
    virtual ~PlexyDesktopView();

    void layout(const QRectF &rect);

public Q_SLOTS:
    void onSessionUpdated(const QString &data);
    void onWidgetClosed(PlexyDesk::AbstractDesktopWidget*);

private:
    class PrivatePlexyDesktopView;
    PrivatePlexyDesktopView *const d;

};

#endif // PLEXYDESKTOPVIEW_H
