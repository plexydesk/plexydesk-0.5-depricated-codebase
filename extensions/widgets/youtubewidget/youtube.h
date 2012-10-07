#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <controllerinterface.h>

class YouTube : public PlexyDesk::ControllerInterface
{
    Q_OBJECT

public:
    YouTube(QObject *object = 0);
    virtual ~YouTube();
    PlexyDesk::AbstractDesktopWidget *defaultView();
    void revokeSession(const QString &key, const QString &value);
    void setViewRect(const QRectF &rect);
};

#endif
