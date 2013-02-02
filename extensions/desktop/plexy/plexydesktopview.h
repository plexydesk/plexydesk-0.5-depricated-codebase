#ifndef PLEXYDESKTOPVIEW_H
#define PLEXYDESKTOPVIEW_H

#include <QGraphicsView>
#include <QAction>

#include <abstractdesktopview.h>

class PlexyDesktopView : public PlexyDesk::AbstractDesktopView
{
    Q_OBJECT
public:
    explicit PlexyDesktopView(QGraphicsScene *parent_scene, QWidget *parent = 0);
    virtual ~PlexyDesktopView();

    void layout(const QRectF &rect);

    void createActions();
    void createMobileActions();


public Q_SLOTS:
    void onSessionUpdated(const QString &data);
    void onWidgetClosed(PlexyDesk::AbstractDesktopWidget*);
    void onTriggered(QAction *action);
    void onButtonClicked();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    class PrivatePlexyDesktopView;
    PrivatePlexyDesktopView *const d;

};

#endif // PLEXYDESKTOPVIEW_H
