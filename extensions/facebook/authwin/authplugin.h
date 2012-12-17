#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <controllerinterface.h>
#include "facebookcontactui.h"
#include "facebookauthenticationwidget.h"

class AuthPlugin : public PlexyDesk::ControllerInterface
{
    Q_OBJECT

public:
    AuthPlugin(QObject *object = 0);

    virtual ~AuthPlugin();

    PlexyDesk::AbstractDesktopWidget *defaultView();

    void revokeSession(const QVariantMap &args);

    void setViewRect(const QRectF &rect);

    void firstRun();

public Q_SLOTS:
    void onDataUpdated(const QVariantMap &map);
    void onFacebookToken(const QString &token);
    void onAddContactCard(const QString &id);

protected:
    void requestFacebookSession();
private:
    FacebookAuthenticationWidget *mWidget;
    FacebookContactUI *mContactUI;
    QString mToken;
};

#endif
