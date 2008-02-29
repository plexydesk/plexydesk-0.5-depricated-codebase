#ifndef RSS_DATA_H
#define RSS_DATA_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>
//#include <QtNetwork>
//#include <QBuffer>
#include <QXmlStreamReader>
#include <QHttp>
#include <QMap>
#include <QList>

class VISIBLE_SYM RssData:public PlexyDesk::AbstractPluginInterface {
    Q_OBJECT
    Q_INTERFACES(PlexyDesk::AbstractPluginInterface)

    public:
    RssData( QObject * object = 0 );
    virtual ~RssData();
    void init();
    virtual QGraphicsItem * item();// {};
    virtual void render( QPainter *p,QRectF r );// {};

    public slots:
    void fetch();
    void finished( int id, bool error );
    void readData( const QHttpResponseHeader & );

    signals:
        void data( QVariant& );

    private:
    void parseXml();

    QXmlStreamReader mXml;
    QString mCurrentTag;
    QString mLinkString;
    QString mTitleString;
    QString mDescString;
    QMap<QString, QVariant> mEntry;
    QList<QVariant> mRssEntries;

    QHttp *mHttp;
    int mConnectionId;
    QTimer *mRssTimer;
};


#endif
