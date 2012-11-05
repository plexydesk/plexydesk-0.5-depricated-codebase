#include "contactlist.h"
#include "contactlistitem.h"

#include <desktopwidget.h>

class ContactList::PrivateContactList
{
public:
    PrivateContactList() {}
    ~PrivateContactList() {}

    QList<ContactListItem*> mContacts;
    PlexyDesk::DesktopWidget *mFrame;
};

ContactList::ContactList(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::ScrollWidget(rect, parent),
    d(new PrivateContactList)
{
    d->mFrame = new PlexyDesk::DesktopWidget(QRectF(rect.x(), rect.y(), 0.0, 24), this);
    d->mFrame->enableDefaultBackground(false);
    d->mFrame->enableShadow(false);
    addWidget(d->mFrame);
}

ContactList::~ContactList()
{
    delete d;
}

void ContactList::addContact(const QString &contactName, const QPixmap &pixmap)
{
    //qDebug() << Q_FUNC_INFO << contactName;
    ContactListItem *contact = new ContactListItem(d->mFrame);
    contact->setName(contactName);
    contact->setPos(0.0, d->mFrame->contentRect().height());
    d->mFrame->setContentRect(QRectF(0.0, boundingRect().y(),
                                     contact->boundingRect().width(),
                                     d->mFrame->contentRect().height() + contact->boundingRect().height()));

}
