#ifndef QMLSVGPROVIDER_H
#define QMLSVGPROVIDER_H

#include <QtDeclarative/QDeclarativeImageProvider>

class QmlSvgProvider : public QDeclarativeImageProvider
{
public:
    QmlSvgProvider(QDeclarativeImageProvider::ImageType type = QDeclarativeImageProvider::Pixmap);
    virtual ~QmlSvgProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    class Private;
    Private *const d;
};

#endif // QMLSVGPROVIDER_H
