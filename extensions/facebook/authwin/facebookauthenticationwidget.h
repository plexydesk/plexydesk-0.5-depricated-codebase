#ifndef FACEBOOKAUTHENTICATIONWIDGET_H
#define FACEBOOKAUTHENTICATIONWIDGET_H

#include <plexy.h>
#include <desktopwidget.h>


class FacebookAuthenticationWidget : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    FacebookAuthenticationWidget(const QRectF &rect, QGraphicsObject *parent = 0);
    ~FacebookAuthenticationWidget();

    QString permissions() const;
    void addPermissions(const QString &perm);

public Q_SLOTS:
    void onLoadFinished(bool ok);
    void onUrlChanged(const QUrl &url);

Q_SIGNALS:
    void facebookToken(const QString &token);

private:
    class PrivateFacebookAuthenticationWidget;
    PrivateFacebookAuthenticationWidget *const d;

};

#endif // FACEBOOKAUTHENTICATIONWIDGET_H
