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
#ifndef Rest_DATA_H
#define Rest_DATA_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>
#include <datainterface.h>
#include <cv.h>
#include <highgui.h>

class VISIBLE_SYM WebCamData :public PlexyDesk::DataPlugin
{
    Q_OBJECT

public:
    WebCamData(QObject * object = 0);
    virtual ~WebCamData();
    void init();
    QVariantMap readAll();

public Q_SLOTS:
    void pushData(QVariant& data);

private Q_SLOTS:
    void grab();
private:
    QRect detectFace(const char* faceData);
    void trackFace();
    void updateHugeImage(const IplImage*);

    class Private;
    Private * const d;
};


#endif
