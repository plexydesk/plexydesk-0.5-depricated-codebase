#ifndef FACEBOOKMESSAGEDIALOG_H
#define FACEBOOKMESSAGEDIALOG_H

#include <plexy.h>
#include <desktopwidget.h>

class FacebookMessageDialog : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    FacebookMessageDialog(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~FacebookMessageDialog();

    QString message() const;

Q_SIGNALS:
    void messageRequested();

private Q_SLOTS:
    void onClicked();

private:
    class PrivateFacebookMessageDialog;
    PrivateFacebookMessageDialog *const d;
};

#endif // FACEBOOKMESSAGEDIALOG_H
