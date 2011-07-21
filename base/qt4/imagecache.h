#ifndef IMAGE_CACHE_H
#define IMAGE_CACHE_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>
#include <plexyconfig.h>
#include <QDeclarativeImageProvider>

namespace PlexyDesk
{
class PLEXYDESK_EXPORT ImageCache : public QDeclarativeImageProvider
{
public:
    ImageCache(QDeclarativeImageProvider::ImageType type = QDeclarativeImageProvider::Pixmap);
    virtual ~ImageCache();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

protected:
    void load(const QString &themename);
    QPixmap get(const QString &name);
    void clear();
    bool drawSvg(QPainter *painter,
         QRectF rect, const QString &str);

private:
    class Private;
    Private *const d;
};
}

#endif

