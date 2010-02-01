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
#include <config.h>


class WebCamData::Private
{
    public:
        Private() {}
        ~Private() {}
        CvCapture * mCaptureData;
        QVariantMap dataMap;
        QTimer * timer;
        IplImage * data;
        CvHaarClassifierCascade* mCascade;
        CvMemStorage* mFaceStore;
        CvSeq* mFaceSeq;
};

WebCamData::WebCamData(QObject * object):d(new Private)
{
    d->data = 0;
    d->mCascade = 0;
    d->mFaceSeq = 0;
    d->mFaceStore = 0;
    d->timer = new QTimer(this);
    qDebug() << Q_FUNC_INFO 
        << ": " << "Start webcam ";
    d->mCaptureData = cvCaptureFromCAM(-1); 
    init();
}

void WebCamData::grab()
{
    if (!d->mCaptureData) {
        return;
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
    d->dataMap.clear();
    d->dataMap["rawImage"] = d->data->imageData;
    d->dataMap["qimage"] = img;
    qDebug() << Q_FUNC_INFO << d->data->width << " x "  << d->data->height;
    d->dataMap["faceRect"] = detectFace(OPENCV_ROOT
            "/share/opencv/haarcascades/haarcascade_frontalface_default.xml");
    Q_EMIT dataReady();
}

void  WebCamData::init()
{
    qDebug() << Q_FUNC_INFO << OPENCV_ROOT;

    if (d->mCaptureData) {
        connect(d->timer, SIGNAL(timeout()), this , SLOT(grab()));
        d->timer->start(1000/30);
    } else {
        qDebug() << Q_FUNC_INFO << ":" << "Capture from webcame failed";
    }

}

QRect WebCamData::detectFace(const char* faceData)
{
    d->mFaceStore = cvCreateMemStorage(0);
    d->mCascade = (CvHaarClassifierCascade*)cvLoad(faceData);
    if (!d->mCascade) {
        qDebug() << Q_FUNC_INFO << ": " << "Error incorrect Haar classifier cascade";
        return QRect();
    }

    CvRect* rect = 0;
    int faceSize = d->data->width/5;
    d->mFaceSeq = cvHaarDetectObjects(d->data,
            d->mCascade, d->mFaceStore, 1.1, 6,
            CV_HAAR_DO_CANNY_PRUNING,
            cvSize(faceSize, faceSize));
   qDebug() << "Number of Faces Detected" << d->mFaceSeq->total;

   if (d->mFaceSeq && d->mFaceSeq->total) {
       rect = (CvRect*) cvGetSeqElem(d->mFaceSeq, 0);
       qDebug() << rect->x << rect->y << rect->width << rect->height;
       return QRect(rect->x, rect->y, rect->width, rect->height);
   }


   return QRect();

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
