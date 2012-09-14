#include "plexygraphicsscene.h"

PlexyGraphicsScene::PlexyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void PlexyGraphicsScene::dropEvent(QDropEvent *event)
{
    qDebug() << Q_FUNC_INFO;
}

void PlexyGraphicsScene::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    event->accept();
}

void PlexyGraphicsScene::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    event->accept();
}
