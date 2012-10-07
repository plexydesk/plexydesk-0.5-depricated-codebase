#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <controllerinterface.h>

class AuthPlugin : public PlexyDesk::ControllerInterface
{
    Q_OBJECT

public:
    AuthPlugin(QObject *object = 0);
    virtual ~AuthPlugin();
    PlexyDesk::AbstractDesktopWidget *defaultView();
    void revokeSession(const QString &key, const QString &value);
    void setViewRect(const QRectF &rect);
};

#endif
