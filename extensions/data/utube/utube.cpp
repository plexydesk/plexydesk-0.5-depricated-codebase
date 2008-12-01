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
#include "utube.h"
#include <desktopwidget.h>

#include <QTimer>

UtubeData::UtubeData( QObject * object )
{
    init();
    mRssTimer = new QTimer( this );
    connect( mRssTimer,SIGNAL( timeout() ),this,SLOT( fetch() ) );
    //mRssTimer->start( 1000*60*60 );
}

void  UtubeData::init()
{
    mHttp = new QHttp( this );
    connect( mHttp, SIGNAL( readyRead( const QHttpResponseHeader & ) ),
            this, SLOT( readData( const QHttpResponseHeader & ) ) );

    connect( mHttp, SIGNAL( requestFinished( int, bool ) ),
            this, SLOT( finished( int, bool ) ) );
    
    if(PlexyDesk::Config::getInstance()->proxyOn){
        QNetworkProxy NtProxy(PlexyDesk::Config::getInstance()->proxyType,
                              PlexyDesk::Config::getInstance()->proxyURL,
                              PlexyDesk::Config::getInstance()->proxyPort,
                              PlexyDesk::Config::getInstance()->proxyUser,
                              PlexyDesk::Config::getInstance()->proxyPasswd
                             );

        mHttp->setProxy(NtProxy);
        QNetworkProxy::setApplicationProxy(NtProxy);
        qDebug() << "UtubeData::init()" << "Proxy state" 
                 << PlexyDesk::Config::getInstance()->proxyOn<<endl;
    }
    
    fetch();
}

UtubeData::~UtubeData()
{
}

/** This method will fetch the
 * rss feed from the given URL */
void UtubeData::fetch()
{
    qDebug() << "UTUBE: Fetching XML..." << endl;

    mXml.clear();
    
    //qDebug() << url.toString();

    mHttp->setHost( "gdata.youtube.com" );
    mConnectionId = mHttp->get(
QString("/feeds/api/videos?vq=%1&max-results=20&orderby=viewCount&alt=rss").
arg("kbfx") );
   
}

void UtubeData::readData( const QHttpResponseHeader &resp )
{
    if (resp.statusCode() != 200){
        mHttp->abort();
        qDebug() << "UTUBE: Error." << endl;
    }else {
        mXml.addData( mHttp->readAll() );
        parseXml();
    }
}

void UtubeData::finished( int id, bool error )
{
    if (error) {
        qDebug() << "UTUBE: Received error during HTTP fetch." <<endl;
        //mRssTimer->start( 2000 );
        //fetch();
    }
    else if (id == mConnectionId) {
        qDebug() << "UTUBE: HTTP fetch Success." <<endl;
        //QVariant rss( mRssEntries );
        //emit data( rss );
        emit success();
    }
}

/** This method will parse the
 * XML and add entries to the QList: mRssEntries*/
void UtubeData::parseXml()
{
    while (!mXml.atEnd()) {
        mXml.readNext();
        if (mXml.isStartElement()) {
            //if (mXml.name() == "item"){
                //mLinkString = mXml.attributes().value("rss:about").toString();
            //}
            mCurrentTag = mXml.name().toString();
        } else if (mXml.isEndElement()) {
            //qDebug() << mXml.name().toString()<<" ::: " << endl;
            if (mXml.name() == "item") {
                
                mVideoID = (mLinkString.split("v=")).at(1);
                mThumbString = "http://img.youtube.com/vi/"+mVideoID+"/1.jpg";
                
                //qDebug() << "UTUBE: " << mThumbString << " : " << endl;

                //mEntry.clear();

                mEntry["title"] = mTitleString;
                mEntry["link"] = mLinkString;
                mEntry["description"] = mDescString;
                mEntry["thumb"] = mThumbString; //97 130
                
                //mThumbString = mThumbString.split(QRegExp("\\s+"));

                QVariant rssitem(mEntry);

                //mRssEntries.append(rssitem);
                
                emit data( rssitem );

                //mTitleString.clear();
                //mLinkString.clear();
                //mDescString.clear();
                //mThumbString.clear();
            }

        } else if (mXml.isCharacters() && !mXml.isWhitespace()) {
            if (mCurrentTag == "title") {
                mTitleString = mXml.text().toString();
            }
            else if (mCurrentTag == "link") {
                mLinkString = mXml.text().toString();
            }
            else if (mCurrentTag == "description") {
                mDescString = mXml.text().toString();
            }
        }
    }

    if (mXml.error() && mXml.error() !=
        QXmlStreamReader::PrematureEndOfDocumentError) {
        qDebug()<< "UTUBE: XML ERROR:" << mXml.lineNumber() << ": " <<
        mXml.errorString();
        mHttp->abort();
        return;
    }

    //QVariant rss(mRssEntries);
    //emit data(rss);
}

QGraphicsItem * UtubeData::item()
{
    /// \todo: null for success compilation only
    return NULL;
}

void UtubeData::render( QPainter *p,QRectF r )
{
}
