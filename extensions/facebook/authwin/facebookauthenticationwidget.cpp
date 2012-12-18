#include "facebookauthenticationwidget.h"
#include <QGraphicsWebView>
#include <controllerinterface.h>
#include <abstractdesktopview.h>

class FacebookAuthenticationWidget::PrivateFacebookAuthenticationWidget
{
public:
    PrivateFacebookAuthenticationWidget() {}
    ~PrivateFacebookAuthenticationWidget() {}

    QGraphicsWebView *mWebView;
    QString mPermissions;
};

FacebookAuthenticationWidget::FacebookAuthenticationWidget(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent),
    d(new PrivateFacebookAuthenticationWidget)
{
    d->mWebView = new QGraphicsWebView(this);
    d->mWebView->resize (rect.size());
    d->mWebView->show();
    d->mWebView->setResizesToContents(false);
    d->mWebView->setGeometry(rect);

    d->mPermissions =
            QString("https://graph.facebook.com/oauth/authorize?client_id=170356722999159&redirect_uri=http://www.facebook.com/connect/login_success.html&type=user_agent&display=popup&response_type=token&scope=read_stream");
    addPermissions("friends_location");
    addPermissions("friends_hometown");

    connect(d->mWebView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(d->mWebView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(onUrlChanged(const QUrl &)));

    d->mWebView->setUrl(QUrl(d->mPermissions));
}

FacebookAuthenticationWidget::~FacebookAuthenticationWidget()
{
    delete d;
}

QString FacebookAuthenticationWidget::permissions() const
{
    return d->mPermissions;
}

void FacebookAuthenticationWidget::addPermissions(const QString &perm)
{
    d->mPermissions += "," + perm ;
}

void FacebookAuthenticationWidget::onLoadFinished(bool ok)
{
    this->setContentRect(d->mWebView->geometry());
}

void FacebookAuthenticationWidget::onUrlChanged(const QUrl &url)
{
    QString stringUrl = url.toString().replace("#", "?");
    QUrl fburl(stringUrl);

    if (not fburl.queryItemValue("access_token").isEmpty()) {
        this->setVisible(false);

        /* save the auth token */
        if(controller() && controller()->viewport()) {
            controller()->viewport()->sessionDataForController(controller()->controllerName(), "access_token", fburl.queryItemValue("access_token"));
            Q_EMIT facebookToken(fburl.queryItemValue("access_token"));
        }

        if (d->mWebView) {
            d->mWebView->setVisible(false);
        }
    }
}
