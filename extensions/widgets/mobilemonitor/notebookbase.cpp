#include "notebookbase.h"
#include "note.h"


class NoteBookBase::PrivateNoteBase
{
public:
    PrivateNoteBase() {}
    ~PrivateNoteBase() {}

    QHash<QString, Note*> mNoteData;
    PlexyDesk::DesktopWidget *mFrame;
};

NoteBookBase::NoteBookBase(const QRectF &rect, QGraphicsObject *parent) :
    ScrollWidget(rect, parent),
    d (new PrivateNoteBase)
{
    d->mFrame = new PlexyDesk::DesktopWidget(QRectF(rect.x(), rect.y(), 0.0, 24), this);
    d->mFrame->enableDefaultBackground(false);
    d->mFrame->enableShadow(false);
    addWidget(d->mFrame);
}

NoteBookBase::~NoteBookBase()
{
    delete d;
}

void NoteBookBase::addNote(const QString &title, const QString &content)
{
    Note *note = new Note(this);
    note->setTitle(title);
    note->setNoteContent(content);

    d->mNoteData[title] = note;
}
