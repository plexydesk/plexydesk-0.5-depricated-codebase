#ifndef FACEBOOKCONTACTUI_H
#define FACEBOOKCONTACTUI_H

#include <plexy.h>
#include <desktopwidget.h>

class FacebookContactUI : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    explicit FacebookContactUI(const QRectF &rect);
    virtual ~FacebookContactUI();

    void setFacebookContactData(QHash<QString, QVariant> data);
    void addContact(const QVariantMap &data);

private:
    class PrivateFacebookContactUI;
    PrivateFacebookContactUI *const d;
};

#endif // FACEBOOKCONTACTUI_H
