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
#ifndef LPHOTOS_DATA_H
#define LPHOTOS_DATA_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>
#include <datainterface.h>

class VISIBLE_SYM LPhotoData : public PlexyDesk::DataPlugin
{
    Q_OBJECT

public:
    LPhotoData(QObject *object = 0);
    virtual ~LPhotoData();
    void init();
public slots:
    void loadImages();
    void nextImage();
    void pushData(QVariant &);
signals:
    void data(QVariant &);

private:
    QBrush paint;
    QString m_dirpath;
    QStringList images;
    int slideCount;
    int currentSlide;
    QTimer *imageTimer;
};


#endif
