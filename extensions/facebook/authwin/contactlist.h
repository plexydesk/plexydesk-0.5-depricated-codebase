#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QObject>
#include <plexy.h>
#include <scrollwidget.h>
#include "contactlistitem.h"

class ContactList : public PlexyDesk::ScrollWidget
{
    Q_OBJECT
public:
    ContactList(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~ContactList();

    void addContact(const QString &id, const QString &contactName, const QString &statusMessage = QString(), const QPixmap &pixmap = QPixmap());

Q_SIGNALS:
    void clicked (QString id);

public Q_SLOTS:
    void clear();
    void filter(const QString &filterString);
    void onItemClicked(ContactListItem *item);

private:
    class PrivateContactList;
    PrivateContactList *const d;
};

#endif // CONTACTLIST_H
