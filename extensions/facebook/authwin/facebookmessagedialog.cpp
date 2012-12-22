#include "facebookmessagedialog.h"
#include <button.h>
#include <lineedit.h>
#include <texteditor.h>

class FacebookMessageDialog::PrivateFacebookMessageDialog
{
public:
    PrivateFacebookMessageDialog() {}
    ~PrivateFacebookMessageDialog() {}

    QString mMessage;
    PlexyDesk::Button *mSearchButton;
    PlexyDesk::TextEditor *mEditor;
};

FacebookMessageDialog::FacebookMessageDialog(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent),
    d(new PrivateFacebookMessageDialog)
{
    d->mSearchButton = new PlexyDesk::Button(this);
    d->mSearchButton->show();
    d->mSearchButton->setPos(rect.width() - 110 , rect.height() - 40);

    connect(d->mSearchButton, SIGNAL(clicked()), this, SLOT(onClicked()));

    this->setContentRect(rect);

    d->mSearchButton->setLabel(tr("Post"));

    d->mEditor = new PlexyDesk::TextEditor(rect.adjusted(0, 0, -15, -68), this);
    d->mEditor->show();
    d->mEditor->setPos(10, 25);
}

FacebookMessageDialog::~FacebookMessageDialog()
{
    delete d;
}

QString FacebookMessageDialog::message() const
{
    return d->mMessage;
}

void FacebookMessageDialog::onClicked()
{
    d->mMessage = d->mEditor->text();

    Q_EMIT messageRequested();

    d->mEditor->setText("");
    this->hide();
}
