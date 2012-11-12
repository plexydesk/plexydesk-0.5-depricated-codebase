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
    ContactListItem *contact = new ContactListItem(d->mFrame);
    contact->setName(contactName);
    contact->setPixmap(pixmap);
    contact->setPos(0.0, d->mFrame->contentRect().height());
    d->mFrame->setContentRect(QRectF(0.0, boundingRect().y(),
                                     contact->boundingRect().width(),
                                     d->mFrame->contentRect().height() + contact->boundingRect().height()));
    d->mContacts.append(contact);
}

void ContactList::clear()
{
    Q_FOREACH(ContactListItem *contact, d->mContacts) {
        contact->hide();
        //contact->setVisible(false);
        d->mFrame->setContentRect(QRectF(0.0, boundingRect().y(),
                                         contact->boundingRect().width(),
                                         d->mFrame->contentRect().height() - contact->boundingRect().height()));
    }

}

void ContactList::filter(const QString &filterString)
{
    //qDebug() << Q_FUNC_INFO << filterString;
    clear();

    qDebug() << Q_FUNC_INFO << "------";
    Q_FOREACH(ContactListItem *contact, d->mContacts) {

        if (contact->name().toLower().contains(filterString)) {

           qDebug() << Q_FUNC_INFO << "Show Contact" << ": " << contact->name();

            int frameHeight = contact->boundingRect().height();

            if (!(d->mFrame->contentRect().height() < 0)) {
                frameHeight = d->mFrame->contentRect().height() + contact->boundingRect().height();

                contact->setPos(0.0, boundingRect().y() + d->mFrame->contentRect().height());
            } else {
                contact->setPos(0.0, boundingRect().y());
            }

            contact->show();

            d->mFrame->setContentRect(QRectF(0.0, boundingRect().y(),
                                             contact->boundingRect().width(),
                                             frameHeight));
            qDebug() << Q_FUNC_INFO << contact->pos() << ":" << contact->isVisible() << ":" << contact->boundingRect();
            qDebug() << Q_FUNC_INFO << d->mFrame->contentRect();
        }
    }
    qDebug() << Q_FUNC_INFO << "+++";
}
