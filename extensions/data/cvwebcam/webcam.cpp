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
#include "webcam.h"
#include <desktopwidget.h>
#include <plexyconfig.h>
#include <QTimer>


class WebCamData::Private
{
    public:
        Private() {}
        ~Private() {}
        CvCapture * mCaptureData;
        QVariantMap dataMap;
        QTimer * timer;
        IplImage * data;
};

WebCamData::WebCamData(QObject * object):d(new Private)
{
    d->data = 0;
    d->timer = new QTimer(this);
    qDebug() << Q_FUNC_INFO 
        << ": " << "Start webcam ";
    d->mCaptureData = cvCaptureFromCAM(-1); 
    init();
}

void WebCamData::grab()
{
    if (d->data) {
    }
    d->data = cvQueryFrame(d->mCaptureData);

    int size = d->data->width * d->data->height;
    unsigned char* end = (unsigned char*) d->data->imageData + (3*size);
    unsigned char* source = (unsigned char*) d->data->imageData;
    unsigned char* dest = new unsigned char[4*size];

    do {
        memcpy (dest, source, 3);
        dest += 4;
        source += 3;
    } while ( source < end);

    QImage img(source, d->data->width, d->data->height, QImage::Format_RGB32);

    qDebug() << Q_FUNC_INFO << d->data->width << " x "  << d->data->height << ":IS "<< img.isNull();
}

void  WebCamData::init()
{

    qDebug() << Q_FUNC_INFO;
    if (d->mCaptureData) {
        connect(d->timer, SIGNAL(timeout()), this , SLOT(grab()));
        d->timer->start(1000/30);
    } else {
        qDebug() << Q_FUNC_INFO << ":" << "Capture from webcame failed";
    }

}

WebCamData::~WebCamData()
{
 qDebug() << Q_FUNC_INFO ;
 cvReleaseCapture(&d->mCaptureData);
 delete d;
}

void WebCamData::pushData(QVariant& arg)
{
}

QVariantMap WebCamData::readAll()
{
    return d->dataMap;
}
