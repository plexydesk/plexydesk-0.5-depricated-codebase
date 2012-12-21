#include "facebookmessagedialog.h"
#include <button.h>
#include <lineedit.h>

class FacebookMessageDialog::PrivateFacebookMessageDialog
{
public:
    PrivateFacebookMessageDialog() {}
    ~PrivateFacebookMessageDialog() {}

    QString mMessage;
    PlexyDesk::Button *mSearchButton;
    PlexyDesk::LineEdit *mLineEdit;
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

    d->mLineEdit = new PlexyDesk::LineEdit(this);
    d->mLineEdit->show();
    d->mLineEdit->setSize(QSizeF(rect.width() - (d->mSearchButton->boundingRect().width() ), 30));
    d->mLineEdit->setPos(10 , rect.height() - 40);
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
    if (!d->mLineEdit)
        return ;

    d->mMessage = d->mLineEdit->text();
    Q_EMIT messageRequested();

    this->hide();
}
