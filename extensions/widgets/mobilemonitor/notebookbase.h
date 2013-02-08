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

    void addNote(const QString &title, const QString &content);

private:
    void createNavBar();

    class PrivateNoteBase;
    PrivateNoteBase *const d;
};

#endif // NOTEBOOKBASE_H
