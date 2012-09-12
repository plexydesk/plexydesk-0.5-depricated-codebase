#ifndef PLEXYDESKTOPVIEW_H
#define PLEXYDESKTOPVIEW_H

#include <QGraphicsView>

class PlexyDesktopView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PlexyDesktopView(QGraphicsScene *scene, QWidget *parent = 0);

    bool setBackgroundSource(const QString& source_id);
    
signals:
    
public slots:

private:
    class PrivatePlexyDesktopView;
    PrivatePlexyDesktopView *const d;

};

#endif // PLEXYDESKTOPVIEW_H
