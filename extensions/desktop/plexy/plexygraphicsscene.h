#ifndef PLEXYGRAPHICSSCENE_H
#define PLEXYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QDragEnterEvent>

class PlexyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PlexyGraphicsScene(QObject *parent = 0);

protected:
    virtual void dropEvent(QDropEvent *event);
    virtual void dragEnterEvent (QDragEnterEvent * event);
    virtual void dragMoveEvent (QDragMoveEvent * event);

signals:

public slots:

};

#endif // PLEXYGRAPHICSSCENE_H
