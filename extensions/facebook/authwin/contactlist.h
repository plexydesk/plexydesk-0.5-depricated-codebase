#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QObject>
#include <plexy.h>
#include <scrollwidget.h>

class ContactList : public PlexyDesk::ScrollWidget
{
    Q_OBJECT
public:
    ContactList(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~ContactList();

    void addContact(const QString &contactName, const QPixmap &pixmap = QPixmap());

public Q_SLOTS:
    void clear();
    void filter(const QString &filterString);

private:
    class PrivateContactList;
    PrivateContactList *const d;
};

#endif // CONTACTLIST_H
