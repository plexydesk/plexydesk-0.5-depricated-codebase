#include "notebookbase.h"
#include "note.h"

#include <button.h>


class NoteBookBase::PrivateNoteBase
{
public:
    PrivateNoteBase() {}
    ~PrivateNoteBase() {}

    QHash<QString, Note*> mNoteData;
    PlexyDesk::DesktopWidget *mFrame;
    PlexyDesk::DesktopWidget *mNavBar;
    PlexyDesk::Button *mNextButton;
    PlexyDesk::Button *mPreviousButton;
};

NoteBookBase::NoteBookBase(const QRectF &rect, QGraphicsObject *parent) :
    ScrollWidget(rect, parent),
    d (new PrivateNoteBase)
{
    d->mFrame = new PlexyDesk::DesktopWidget(QRectF(rect.x(), rect.y(), 0.0, 24), this);
    d->mFrame->enableDefaultBackground(false);
    d->mFrame->enableShadow(false);

    createNavBar();

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

void NoteBookBase::createNavBar()
{
    d->mNavBar = new PlexyDesk::DesktopWidget(QRectF(0.0, 0.0, this->boundingRect().width(), 128), this);
    d->mNavBar->enableDefaultBackground(true);
    d->mNavBar->enableWindowMode(false);
    d->mNavBar->enableShadow(true);
    d->mNavBar->setFlag(QGraphicsItem::ItemIsMovable, false);

    d->mNavBar->setPos(0.0, this->boundingRect().height() - d->mNavBar->boundingRect().height());

    d->mNextButton = new PlexyDesk::Button(d->mNavBar);
    d->mPreviousButton = new PlexyDesk::Button(d->mNavBar);

    d->mNextButton->setLabel("Next");
    d->mPreviousButton->setLabel("Back");

    d->mNextButton->setSize(QSize(242,108));
    d->mPreviousButton->setSize(QSize(242,108));

    d->mNextButton->setPos(d->mNavBar->boundingRect().width() - d->mNextButton->boundingRect().width() - 5.0, 10.0);
    d->mPreviousButton->setPos(5.0, 10.0);
}
