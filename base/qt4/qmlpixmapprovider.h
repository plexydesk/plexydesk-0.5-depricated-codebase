#ifndef QMLPIXMAPPROVIDER_H
#define QMLPIXMAPPROVIDER_H

#include <QDeclarativeImageProvider>

class QmlPixmapProvider : public QDeclarativeImageProvider
{
public:
    QmlPixmapProvider(QDeclarativeImageProvider::ImageType type = QDeclarativeImageProvider::Pixmap);
    virtual ~QmlPixmapProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    class Private;
    Private *const d;
};

#endif // QMLPIXMAPPROVIDER_H
