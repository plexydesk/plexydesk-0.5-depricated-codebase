/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
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
#include "rss.h"
#include <desktopwidget.h>

RssData::RssData( QObject * object )
{
    init();
    mRssTimer = new QTimer( this );
    connect( mRssTimer,SIGNAL( timeout() ),this,SLOT( fetch() ) );
    mRssTimer->start( 1000*60*60 );
}

void  RssData::init()
{
    mHttp = new QHttp( this );
    connect( mHttp, SIGNAL( readyRead( const QHttpResponseHeader & ) ),
            this, SLOT( readData( const QHttpResponseHeader & ) ) );

    connect( mHttp, SIGNAL( requestFinished( int, bool ) ),
            this, SLOT( finished( int, bool ) ) );
    fetch();
}

RssData::~RssData()
{
}

/** This method will fetch the
 * rss feed from the given URL */
void RssData::fetch()
{
    qDebug() << "RSS: Fetching XML..." << endl;

    mXml.clear();

    QUrl url( "mHttp://labs.trolltech.com/blogs/feed" );

    mHttp->setHost( url.host() );
    mConnectionId = mHttp->get( url.path() );
}

void RssData::readData( const QHttpResponseHeader &resp )
{
    if (resp.statusCode() != 200){
        mHttp->abort();
        qDebug() << "RSS: Error." << endl;
    }else {
        mXml.addData( mHttp->readAll() );
        parseXml();
    }
}

void RssData::finished( int id, bool error )
{
    if (error) {
        qDebug() << "RSS: Received error during HTTP fetch." <<endl;
    }
    else if (id == mConnectionId) {
        qDebug() << "RSS: HTTP fetch Success." <<endl;
        QVariant rss( mRssEntries );
        emit data( rss );
    }
}

/** This method will parse the
 * XML and add entries to the QList: mRssEntries*/
void RssData::parseXml()
{
    while (!mXml.atEnd()) {
        mXml.readNext();
        if (mXml.isStartElement()) {
            if (mXml.name() == "item"){
                mLinkString = mXml.attributes().value("rss:about").toString();
            }
            mCurrentTag = mXml.name().toString();
        } else if (mXml.isEndElement()) {
            if (mXml.name() == "item") {
                qDebug() << mTitleString << " : " << endl;

                //mEntry.clear();

                mEntry["title"] = mTitleString;
                mEntry["link"] = mLinkString;
                mEntry["description"] = mDescString;

                QVariant rssitem(mEntry);

                mRssEntries.append(rssitem);

                mTitleString.clear();
                mLinkString.clear();
            }

        } else if (mXml.isCharacters() && !mXml.isWhitespace()) {
            if (mCurrentTag == "title") {
                mTitleString += mXml.text().toString();
            }
            else if (mCurrentTag == "link") {
                mLinkString += mXml.text().toString();
            }
            else if (mCurrentTag == "description") {
                mDescString += mXml.text().toString();
            }
        }
    }

    if (mXml.error() && mXml.error() !=
        QXmlStreamReader::PrematureEndOfDocumentError) {
        qDebug()<< "XML ERROR:" << mXml.lineNumber() << ": " <<
        mXml.errorString();
        mHttp->abort();
        return;
    }

    //QVariant rss(mRssEntries);
    //emit data(rss);
}

QGraphicsItem * RssData::item()
{
}

void RssData::render( QPainter *p,QRectF r )
{
}

Q_EXPORT_PLUGIN2(RssData,RssData)
#include "rss.moc"
