#ifndef M_NOTE_H
#define M_NOTE_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <plexy.h>
#include <style.h>

class Note : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Note(QGraphicsObject *parent = 0);
    virtual ~Note();

    virtual QRectF boundingRect() const;

    void setTitle(const QString &title);
    void setNoteContent(const QString &status);
    void setID(const QString &id);
    QString title() const;
    QString id();
    QString noteContent() const;

    void setPixmap(const QPixmap &pixmap);

Q_SIGNALS:
    void clicked(Note *item);

public Q_SLOTS:
    void onClicked();

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    class PrivateNote;
    PrivateNote *const d;
};

#endif // M_NOTE_H
