#ifndef IMAGE_CACHE_H
#define IMAGE_CACHE_H

#include <QtCore>
#include <QtGui>
#include <plexy.h>
#include <plexyconfig.h>

namespace PlexyDesk
{
class PLEXYDESK_EXPORT ImageCache : public QObject
{
    Q_OBJECT
public:
    virtual ~ImageCache();
    void load(const QString &themename);
    QPixmap get(const QString &name);
    static ImageCache *instance();
    void clear();
    bool drawSvg(QPainter *painter,
            QRectF rect, const QString &str);

private:
    ImageCache() ;
    class Private;
    Private * const d ;
    static ImageCache *staticObject;
};
}

#endif

