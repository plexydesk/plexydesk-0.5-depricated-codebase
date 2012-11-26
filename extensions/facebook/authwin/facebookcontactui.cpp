#include "facebookcontactui.h"
#include <scrollwidget.h>
#include <button.h>
#include <lineedit.h>
#include "contactlist.h"

class FacebookContactUI::PrivateFacebookContactUI {
public:
    PrivateFacebookContactUI() {}
    ~PrivateFacebookContactUI() {}

    QHash<QString, QVariant> mData;
    ContactList *mScrollView;
    PlexyDesk::Button *mSearchButton;
    PlexyDesk::LineEdit *mSearchBox;
};

FacebookContactUI::FacebookContactUI(const QRectF &rect) :
    PlexyDesk::DesktopWidget(rect),
    d (new PrivateFacebookContactUI)
{
    enableDefaultBackground(true);

    QRectF scrollRect = QRect(0.0, 24.0, rect.width(), rect.height() - 90);
    d->mScrollView = new ContactList(scrollRect, this);
    d->mScrollView->setVisible(true);
    d->mScrollView->enableDefaultBackground(false);
    d->mScrollView->enableShadow(false);

    d->mSearchBox = new PlexyDesk::LineEdit(this);
    d->mSearchBox->show();
    d->mSearchBox->setPos(24 , rect.height() - 48);

    d->mSearchButton = new PlexyDesk::Button(this);
    d->mSearchButton->show();
    d->mSearchButton->setLabel(tr("Search"));
    d->mSearchButton->setPos(d->mSearchBox->boundingRect().width(), rect.height() - 48);

    connect(d->mSearchBox, SIGNAL(text(QString)), d->mScrollView, SLOT(filter(QString)));
}

FacebookContactUI::~FacebookContactUI()
{
    d->mData.clear();
    delete d;
}

void FacebookContactUI::setFacebookContactData(QHash<QString, QVariant> data)
{
   d->mData = data;

   Q_FOREACH(const QString &name, data.keys()) {
       d->mScrollView->addContact(name);
   }
}

void FacebookContactUI::addContact(const QVariantMap &data)
{
    d->mScrollView->addContact(data["name"].toString(), data["message"].toString(), data["picture"].value<QPixmap>());
}
