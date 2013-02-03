#ifndef NOTEBOOKBASE_H
#define NOTEBOOKBASE_H

#include <QObject>
#include <scrollwidget.h>

class NoteBookBase : public PlexyDesk::ScrollWidget
{
    Q_OBJECT
public:
    NoteBookBase(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~NoteBookBase();
    
signals:
    
public slots:
    
};

#endif // NOTEBOOKBASE_H
