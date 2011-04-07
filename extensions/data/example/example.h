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
#ifndef Example_DATA_H
#define Example_DATA_H

#include <QtCore>
#include <plexy.h>
#include <backdropinterface.h>
#include <abstractplugininterface.h>
#include <QtNetwork>
#include <datainterface.h>

class VISIBLE_SYM ExampleData : public PlexyDesk::DataPlugin
{
    Q_OBJECT

public:
    ExampleData(QObject *object = 0);
    virtual ~ExampleData();
    void init();
public slots:
    /* ID tells you the Qhttp id which youare pocessing
     */
    void loadCallback(int ID, bool status);
signals:
    void data(QVariant &);

private:
    QPixmap bg;
    QImage img;
    QBrush paint;
    QString searchkey;
    QHttp *http;
    QImage newWall;
    QStringList images;
    int requestID;
    int dataID;
    int slideCount;
    int currentSlide;
    QTimer *imageTimer;
};


#endif
