#include "facebookcontactui.h"
#include <scrollwidget.h>
#include <button.h>
#include <lineedit.h>
#include "contactlist.h"

class FacebookContactUI::PrivateFacebookContactUI {
public:
    PrivateFacebookContactUI() {}
    ~PrivateFacebookContactUI() {}

    QHash<QString, QVariant> mData;
    ContactList *mScrollView;
    PlexyDesk::Button *mSearchButton;
    PlexyDesk::LineEdit *mSearchBox;
};

FacebookContactUI::FacebookContactUI(const QRectF &rect) :
    PlexyDesk::DesktopWidget(rect),
    d (new PrivateFacebookContactUI)
{
    enableDefaultBackground(true);

    QRectF scrollRect = QRect(0.0, 24.0, rect.width(), rect.height() - 80);
    d->mScrollView = new ContactList(scrollRect, this);
    d->mScrollView->setVisible(true);
    d->mScrollView->enableDefaultBackground(false);
    d->mScrollView->enableShadow(false);
//    d->mScrollView->hide();

    d->mSearchBox = new PlexyDesk::LineEdit(this);
    d->mSearchBox->setSize (QSizeF(rect.width() - 40, 32));
    d->mSearchBox->show();
    d->mSearchBox->setPos( (rect.width() / 2) - (d->mSearchBox->boundingRect().width() / 2) , rect.height() - 45);

    connect(d->mSearchBox, SIGNAL(text(QString)), d->mScrollView, SLOT(filter(QString)));
    connect(d->mScrollView, SIGNAL(clicked(QString)), this, SLOT(onViewClicked(QString)));

    setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

FacebookContactUI::~FacebookContactUI()
{
    d->mData.clear();
    delete d;
}

void FacebookContactUI::setFacebookContactData(QHash<QString, QVariant> data)
{
   d->mData = data;

   Q_FOREACH(const QString &name, data.keys()) {
      // d->mScrollView->addContact(name);
   }
}

void FacebookContactUI::addContact(const QVariantMap &data)
{
    d->mScrollView->addContact(data["id"].toString(), data["name"].toString(), data["message"].toString(), data["picture"].value<QPixmap>());
}

void FacebookContactUI::onViewClicked(QString id)
{
    qDebug() << Q_FUNC_INFO << id;
    Q_EMIT addContactCard(id);
}

void FacebookContactUI::paintFrontView(QPainter *painter, const QRectF &rect)
{
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect, 6.0, 6.0);

    painter->fillPath(backgroundPath, QColor(235, 235, 235));

    if (d->mSearchBox) {
        QRectF searchBoxRect = QRectF (0.0, d->mScrollView->boundingRect().height() + 28,
                                       rect.width(), rect.height());

        QRectF searchBoxLineRect = QRectF (0.0, d->mScrollView->boundingRect().height() + 29,
                                       rect.width(), rect.height());

        QPen pen = QPen(QColor(220, 220, 220), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);

        QLinearGradient linearGrad(searchBoxRect.topRight(), searchBoxRect.bottomRight());

        linearGrad.setColorAt(0, QColor(111, 111, 111));
        linearGrad.setColorAt(1, QColor(105, 105, 105));

        painter->fillRect(searchBoxRect, linearGrad);
        painter->drawLine(searchBoxRect.topRight(), searchBoxRect.topLeft());
        pen = QPen(QColor(88, 88, 88), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(pen);
        painter->drawLine(searchBoxLineRect.topRight(), searchBoxLineRect.topLeft());
    }
}
