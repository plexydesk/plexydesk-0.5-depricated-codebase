#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <plexy.h>
#include <style.h>


class ContactListItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ContactListItem(QGraphicsObject *parent = 0);
    virtual ~ContactListItem();

    virtual QRectF boundingRect() const;
    void setName(const QString &name);
    void setStatusMessage(const QString &status);
    void setID(const QString &id);
    QString name() const;
    QString id();
    QString statusMessage() const;
    void setPixmap(const QPixmap &pixmap);

Q_SIGNALS:
    void clicked(ContactListItem *item);

public Q_SLOTS:
    void onClicked();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    class PrivateContactListItem;
    PrivateContactListItem *const d;
};

#endif // CONTACTLISTITEM_H
