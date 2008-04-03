/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal Priyadarshana <llahiru@gmail.com>
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
#include "localphotos.h"
#include <desktopwidget.h>
#include <plexyconfig.h>


LPhotoData::LPhotoData(QObject * object)
{
	m_dirpath = QDir::homePath();
        slideCount = 0;
        currentSlide = 0;
        imageTimer = new QTimer(this);
        connect(imageTimer,SIGNAL(timeout()),this,SLOT(nextImage()));
	init();
}

void  LPhotoData::init()
{
    if(imageTimer->isActive())
        imageTimer->stop();

    images.clear();

    QDir imgdir(m_dirpath);
    imgdir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList infolist = imgdir.entryInfoList();

    for(int i=0; i<infolist.count(); i++){
	QImage test_img(infolist.at(i).filePath());
    	if (!test_img.isNull()) {
	    images << infolist.at(i).filePath();
	}
    }
    slideCount = images.count();

    if(slideCount>0){
    	if(!imageTimer->isActive())
        	imageTimer->start(3000);
    }
}

LPhotoData::~LPhotoData()
{
}

void LPhotoData::nextImage()
{
    if(slideCount>0){
        loadImages();
        currentSlide++;
        if (currentSlide > slideCount-1) {
            currentSlide = 0;
            //imageTimer->stop();
        }
    }
}

void LPhotoData::pushData(QVariant& str)
{
    qDebug()<<"pushData: "<<str.toString()<<endl;
    m_dirpath = str.toString();
    init();
}

void LPhotoData::loadImages()
{
    QFile file(images.at(currentSlide));

    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Invalid File"<<endl;
        return;
    }

    QByteArray img_data = file.readAll();

    QImage test_img(QImage::fromData(img_data));

    if (!test_img.isNull()) {
        QVariant image(img_data);
	emit data(image);
    }else {
        qDebug()<<"Invalid Image data"<<endl;
    }
}


#include "localphotos.moc"
