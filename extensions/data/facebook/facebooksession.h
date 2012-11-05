#ifndef FACEBOOKSESSION_H
#define FACEBOOKSESSION_H

#include <QObject>
#include <QtCore>
#include <plexy.h>
#include <datasource.h>


class FacebookSession : public PlexyDesk::DataSource
{
    Q_OBJECT
public:
    explicit FacebookSession(QObject *parent = 0);

    virtual QVariantMap readAll();

public Q_SLOTS:
    virtual void setArguments(QVariant &args);
    virtual void onFriendListReady();

private:
    class Private;
    Private *const d;
};

#endif // FACEBOOKSESSION_H
