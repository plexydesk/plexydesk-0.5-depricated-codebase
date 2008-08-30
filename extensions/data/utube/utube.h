/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#ifndef UTUBE_DATA_H
#define UTUBE_DATA_H

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

class VISIBLE_SYM UtubeData : public PlexyDesk::DataPlugin 
{
    Q_OBJECT
    
    public:
        UtubeData( QObject * object = 0 );
        virtual ~UtubeData();
    void init();
    virtual QGraphicsItem * item();// {};
    virtual void render( QPainter *p,QRectF r );// {};

    public slots:
    void fetch();
    void finished( int id, bool error );
    void readData( const QHttpResponseHeader & );

    signals:
        void data( QVariant& );
        void success();

    private:
    void parseXml();

    QXmlStreamReader mXml;
    QString mCurrentTag;
    QString mLinkString;
    QString mTitleString;
    QString mDescString;
    QString mThumbString;
    
    QString mVideoID;
    
    QMap<QString, QVariant> mEntry;
    QList<QVariant> mRssEntries;

    QHttp *mHttp;
    int mConnectionId;
    QTimer *mRssTimer;
};


#endif
