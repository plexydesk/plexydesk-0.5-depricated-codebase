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
    CvRect faceRect;
    bool hasFace;

    //
    IplImage* hsvImage;
    IplImage* mask;
    IplImage* hueImage;
    IplImage* prob;

    CvHistogram* histogram;

};

WebCamData::WebCamData(QObject * object): d(new Private)
{
    d->data = 0;
    d->hasFace = false;
    d->mCascade = 0;
    d->mFaceSeq = 0;
    d->mFaceStore = 0;

    d->hsvImage = 0;
    d->mask = 0;
    d->hueImage = 0;
    d->prob = 0;
    d->histogram = 0;
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
    if (!d->data) {
        return;
    }
    /*
        int size = d->data->width * d->data->height;
        unsigned char* end = (unsigned char*) d->data->imageData + (3*size);
        unsigned char* source = (unsigned char*) d->data->imageData;
        unsigned char* dest = new unsigned char[4*size];

        do {
            memcpy (dest, source, 3);
            dest += 4;
            source += 3;
        } while ( source < end);
    */
    //// QImage img(source, d->data->width, d->data->height, QImage::Format_RGB32);
    detectFace(OPENCV_ROOT
               "/share/opencv/haarcascades/haarcascade_frontalface_default.xml");
}

void  WebCamData::init()
{
    qDebug() << Q_FUNC_INFO << OPENCV_ROOT;

    if (d->mCaptureData) {
        connect(d->timer, SIGNAL(timeout()), this , SLOT(grab()));
        d->timer->start(1000 / 30);
    } else {
        qDebug() << Q_FUNC_INFO << ":" << "Capture from webcame failed";
    }

    d->mCascade = (CvHaarClassifierCascade*)cvLoad(OPENCV_ROOT
                  "/share/opencv/haarcascades/haarcascade_frontalface_default.xml");

    d->mFaceStore = cvCreateMemStorage(0);

}

QRect WebCamData::detectFace(const char* faceData)
{

    if (d->hasFace) {
        trackFace();
        return QRect();
    }
    if (!d->mCascade) {
        qDebug() << Q_FUNC_INFO << ": " << "Error incorrect Haar classifier cascade";
        return QRect();
    }

    CvRect* rect = 0;
    int faceSize = d->data->width / 5;
    d->mFaceSeq = cvHaarDetectObjects(d->data,
                                      d->mCascade, d->mFaceStore, 1.1, 6,
                                      CV_HAAR_DO_CANNY_PRUNING,
                                      cvSize(faceSize, faceSize));
//   qDebug() << "Number of Faces Detected" << d->mFaceSeq->total;

    if (d->mFaceSeq && d->mFaceSeq->total) {
        rect = (CvRect*) cvGetSeqElem(d->mFaceSeq, 0);
        d->hasFace = true;

        int radius = cvRound((rect->width + rect->height) * 0.25);
        CvPoint center;
        center.x = cvRound(rect->x + rect->width * 0.5);
        center.y = cvRound(rect->y + rect->height * 0.5);

        //qDebug() << "Radius : " << radius << " X: " << center.x << "Y : " << center.y;
        //histogram

        float max = 0.f;
        float range[]  = {0, 180};
        float* ranges = range;
        int bins = 30;
        d->hsvImage = cvCreateImage(cvGetSize(d->data), 8, 3);
        d->hueImage = cvCreateImage(cvGetSize(d->data), 8, 1);
        d->mask = cvCreateImage(cvGetSize(d->data), 8, 1);
        d->prob = cvCreateImage(cvGetSize(d->data), 8, 1);
        d->histogram = cvCreateHist(1, &bins, CV_HIST_ARRAY, &ranges, 1);

        updateHugeImage(d->data);

        cvSetImageROI(d->hueImage, *rect);
        cvSetImageROI(d->mask, *rect);
        cvCalcHist(&d->hueImage, d->histogram, 0, d->mask);
        cvGetMinMaxHistValue(d->histogram, 0, &max, 0, 0);
        cvConvertScale(d->histogram->bins, d->histogram->bins, max ? 255.0 / max : 0, 0);
        cvResetImageROI(d->hueImage);
        cvResetImageROI(d->mask);

        d->faceRect = *rect;

        /*
                      */
    }


    return QRect();

}

void WebCamData::trackFace()
{
    CvConnectedComp comps;
    updateHugeImage(d->data);

    cvCalcBackProject(&d->hueImage, d->prob, d->histogram);
    cvAnd(d->prob, d->mask, d->prob, 0);
    CvBox2D box;
    cvCamShift(d->prob, d->faceRect,
               cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1),
               &comps, &box);

    d->faceRect = comps.rect;


    int radius = cvRound((d->faceRect.width + d->faceRect.height) * 0.25);
    CvPoint center;
    center.x = cvRound(d->faceRect.x + d->faceRect.width * 0.5);
    center.y = cvRound(d->faceRect.y + d->faceRect.height * 0.5);
/*
    qDebug() << Q_FUNC_INFO
    << comps.rect.x
    << comps.rect.y
    << comps.rect.width
    << comps.rect.height
    << box.angle
    << center.x
    << center.y
    << radius;
    */
    d->dataMap.clear();
    d->dataMap["z"] = QVariant(radius);
    d->dataMap["x"] = QVariant(center.x);
    d->dataMap["y"] = QVariant(center.y);
    d->dataMap["angle"] = QVariant(box.angle);
    Q_EMIT dataReady();

}


void WebCamData::updateHugeImage(const IplImage* img)
{
    cvCvtColor(img, d->hsvImage, CV_BGR2HSV);
    cvInRangeS(d->hsvImage, cvScalar(0, 55, MIN(65, 256), 0),
               cvScalar(180, 256, MAX(65, 255), 0), d->mask);
    cvSplit(d->hsvImage, d->hueImage, 0, 0, 0);
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
