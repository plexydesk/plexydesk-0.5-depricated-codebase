#include "photowidget.h"

#include <svgprovider.h>

#include <button.h>

class PhotoWidget::PrivatePhotoWidget
{
public:
    PrivatePhotoWidget() {}
    ~PrivatePhotoWidget() {}

    QGraphicsProxyWidget *mProxyWidget;
    QLineEdit *mEditor;
    PlexyDesk::Button *mButton;
};

PhotoWidget::PhotoWidget(const QRectF &rect) :
    PlexyDesk::DesktopWidget(rect), d (new PrivatePhotoWidget)
{
   enableShadow(true);
   setLabelName("bbwidget");

   d->mProxyWidget = new QGraphicsProxyWidget(this);
   d->mEditor = new QLineEdit(0);
   d->mEditor->setStyleSheet("font: 30px; background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffffff, stop: 1 #E5EAEE); border : 0");

   d->mProxyWidget->setWidget(d->mEditor);

   QSizeF size = rect.size();

   size.setHeight(rect.height() / 2);

   d->mProxyWidget->resize(size);
   d->mProxyWidget->setMinimumSize(size);

   d->mProxyWidget->show();
   d->mEditor->move(0.0, 0.0);
   d->mProxyWidget->setPos(0.0, 24.0);

   d->mButton = new PlexyDesk::Button(this);

   d->mButton->setLabel("Authorize Clients");
   d->mButton->show();
   d->mButton->setPos (24.0, d->mEditor->height() + 28);
   d->mButton->setSize(QSize(rect.width() - 48, rect.height() - (d->mEditor->size().height() + 28)));

   connect (d->mButton, SIGNAL(clicked()), this, SLOT(onClicked()));
}

PhotoWidget::~PhotoWidget()
{
    delete d;
}

void PhotoWidget::setContentImage(const QPixmap &pixmap)
{
    mContentPixmap = pixmap;
    QRectF pixmapRect = pixmap.rect();

    /*calculate the new height for the content rect width */
    float pixmapHeight = (pixmapRect.height() / pixmapRect.width() ) * contentRect().width();
    pixmapRect.setHeight(pixmapHeight);
    pixmapRect.setWidth(contentRect().width());
    setContentRect(pixmapRect);

    update();
}

void PhotoWidget::paintRotatedView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintRotatedView(painter, rect);
}

void PhotoWidget::paintFrontView(QPainter *painter, const QRectF &rect)
{
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setPen(QColor(255, 255, 255));

    painter->setOpacity(1.0);

    if (mContentPixmap.isNull()) {
        PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
        painter->drawText(QRect(0, 0, 190, 180), Qt::AlignCenter | Qt::AlignRight,
                          QLatin1String("Drag and Drop a Photo Here!"));
        PlexyDesk::DesktopWidget::paintFrontView(painter, rect);
    } else {
        //experimental photo frame around the image
        //painter->fillRect(rect.x(), rect.y(), rect.width(), rect.height(),  QColor(255, 255, 255));
        //float frameWidth = 4.0;
        //painter->translate(frameWidth, frameWidth);
        //painter->drawPixmap(rect.x(), rect.y(), rect.width() - (frameWidth * 2), rect.height() - (frameWidth * 2), mContentPixmap);
        painter->drawPixmap(rect.x(), rect.y(), rect.width(), rect.height(), mContentPixmap);
    }
}

void PhotoWidget::paintDockView(QPainter *painter, const QRectF &rect)
{
    PlexyDesk::DesktopWidget::paintDockView(painter, rect);
}


void PhotoWidget::onClicked()
{
    if (d->mEditor) {
        Q_EMIT approvedToken (d->mEditor->text());
    }
}
