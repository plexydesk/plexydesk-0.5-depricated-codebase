/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : SLIIT
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

#include <plexy.h>
#include "googleweatherwidget.h"
#include <math.h>

#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>
#include <QColor>
#include "socialqdbusplugindata.h"
#include <QTimer>
namespace PlexyDesk
{
googleweatherWidget::googleweatherWidget (const QRectF &rect, QWidget *widget) :
    DesktopWidget(rect, widget)
{
    shade = 0;
    moveY = 0;
    setPath(applicationDirPath() +"/theme/skins/default/widget/default/googleweather/");
    setDockImage(QPixmap(prefix + "icon.png"));
    drawWidget();

    clip = QRectF(10, 10, rect.width()-40, rect.height()-20);
    view = QRectF(0, 0, rect.width()-80, 0);
    initD = 0;
    DS_EXTF = 0;
    initTimer = 1;
    weather = new SocialQDBusPluginData();

}
googleweatherWidget::~googleweatherWidget ()
{
}
void googleweatherWidget::setPath(QString str)
{
    prefix = str+"/";
}
//----------------------------------------
void googleweatherWidget::data()
{
    initTimer = 0;
    initD++;
    if (initD == 1)
    {
        QList<QVariant> dataPkg;
        QVariantMap args;
        weatherKeys.clear();
        weatherValues.clear();
        //TODO : adding capability to change the station
        //args["station"] = QVariant("colombo");
        weatherdata = weather->getData("weatherplugin", "getweather", args);
        //unwrapping the QVariantMap
        weatherKeys = weatherdata.keys();
        if (weatherKeys[0] != "Network Error")
        {
            for (int i = 0; i<weatherKeys.length(); i++)
            {
                dataPkg<<weatherdata[weatherKeys.at(i)];
                if (dataPkg[i].canConvert<QStringList>())
                    weatherValues<<dataPkg[i].value<QStringList>();
            }
        }else{
            dataPkg<<weatherdata[weatherKeys.at(0)];
            if (dataPkg[0].canConvert<QString>())
            {
                for (int i = 0; i<26; i++)
                {
                    weatherValues<<dataPkg[0].value<QStringList>();
                }
            }
        }
        //unwrapping completed..
    }
}
void googleweatherWidget::tempInCel(QPainter *p, QString temp, int x, int y, bool c, int size)
{
    bool valid;
    int t = temp.toInt(&valid, 10);

    int ts;
    if (!c)
    {
        t = (t-32)/1.8;
    }
    temp = QString::number(t);
    if (valid)
        p->setPen(QColor(175+t*2, 120-t*3, 120-t*3));
    else
        p->setPen(QColor(255, 0, 0));
    if (size < 21 && size > 9)
        ts = size - 5;
    else if (size < 9)
        ts = size - 3;
    else
        ts = size -15;
    p->setFont(QFont("SansSerif", size, QFont::Bold));
    p->drawText(clip.x()+x, view.y()+y, temp+" C");
    p->setFont(QFont("SansSerif", ts, QFont::Bold));
    if (valid && size > 20) {
        if (t > 9)
            p->drawText(clip.x()+x+65, view.y()+y-25, "o");
        else if (t > -1)
            p->drawText(clip.x()+x+35, view.y()+y-28, "o");
        else if (t > -10)
            p->drawText(clip.x()+x+55, view.y()+y-28, "o");
        else
            p->drawText(clip.x()+x+75, view.y()+y-28, "o");

    }else if (valid && size < 21)
    {
        if (size < 12)
        {
            if (t > 9)
                p->drawText(clip.x()+x+21, view.y()+y-8, "o");
            else if (t > -1)
                p->drawText(clip.x()+x+11, view.y()+y-8, "o");
            else if (t > -10)
                p->drawText(clip.x()+x+18, view.y()+y-8, "o");
            else
                p->drawText(clip.x()+x+28, view.y()+y-8, "o");
        }
        else
        {
            if (t > 9)
                p->drawText(clip.x()+x+24, view.y()+y-8, "o");
            else if (t > -1)
                p->drawText(clip.x()+x+11, view.y()+y-8, "o");
            else if (t > -10)
                p->drawText(clip.x()+x+20, view.y()+y-8, "o");
            else
                p->drawText(clip.x()+x+32, view.y()+y-8, "o");
        }
    }
    p->setFont(QFont("SansSerif", 10, QFont::Normal));

}
void googleweatherWidget::drawSet(QPainter *p)
{
    QString current, humidity, temp, wind, image, curr_date;
    current = weatherValues.at(0);
    humidity = weatherValues.at(1);
    temp = weatherValues.at(2);
    wind = weatherValues.at(3);
    curr_date = weatherValues.at(25);
    QStringList f_day, f_condition, f_lTemp, f_hTemp, f_image;

    f_day<<weatherValues.at(4)<<weatherValues.at(5)<<weatherValues.at(6)<<weatherValues.at(7);
    f_condition<<weatherValues.at(8)<<weatherValues.at(9)<<weatherValues.at(10)<<weatherValues.at(11);
    f_lTemp<<weatherValues.at(12)<<weatherValues.at(13)<<weatherValues.at(14)<<weatherValues.at(15);
    f_hTemp<<weatherValues.at(16)<<weatherValues.at(17)<<weatherValues.at(18)<<weatherValues.at(19);
    f_image<<weatherValues.at(21)<<weatherValues.at(22)<<weatherValues.at(23)<<weatherValues.at(24);
    image = weatherValues.at(20);


    p->setPen(QColor(255, 255, 255, 255));
    p->setFont(QFont("SansSerif", 15, QFont::Bold));
    p->drawLine(clip.x()+w_ImageDock.width()+57, view.y()+45, clip.x()+w_ImageDock.width()+251, view.y()+45);
    p->drawText(clip.x()+w_ImageDock.width()+60, view.y()+40, "Google Weather");
    p->setPen(QColor(0, 255, 255, 127));
    p->setFont(QFont("SansSerif", 10, QFont::Normal));
    if (current != "Data not recieved")
    {
        p->drawText(clip.x()+10, view.y()+65, "City: Colombo");
        p->setPen(QColor(200, 255, 100, 127));

        //------------current conditions --------------
        QFont tempFont = QFont("OldEnglish", 14, QFont::DemiBold);
        tempFont.setStyleStrategy(QFont::OpenGLCompatible);
        p->setFont(tempFont);
        p->drawText(clip.x()+20, view.y()+155, current);
        p->setFont(QFont("SansSerif", 10, QFont::Normal));
        p->setPen(QColor(58, 136, 253));
        p->drawText(clip.x()+250, view.y()+90, "Today : " + curr_date);
        p->drawText(clip.x()+250, view.y()+130, humidity);
        p->drawText(clip.x()+250, view.y()+145, wind);
        tempInCel(p, temp, 10, 120, true, 35);
        p->drawImage(QRect(clip.x()+140, view.y()+55, 100, 100), QImage(prefix+image+".png"));
        //-----------end of current conditions-----------

        //////////////// forecst information //////////////////////

        p->setFont(QFont("SansSerif", 10, QFont::Normal));
        p->setPen(QColor(50, 50, 50));
        p->drawLine(clip.x()+5, view.y()+175, clip.x()+402, view.y()+175);
        p->drawLine(clip.x()+140, view.y()+175, clip.x()+140, view.y()+270);
        p->drawLine(clip.x()+270, view.y()+175, clip.x()+270, view.y()+270);
        for (int i = 1; i<4; i++)
        {
            p->setPen(QColor(0, 234, 255));
            p->drawText(clip.x()+15+(i-1)*130, view.y()+190, getDay(f_day.at(i)));
            p->setPen(QColor(56, 193, 124));
            p->drawText(clip.x()+15+(i-1)*130, view.y()+267, f_condition.at(i));
            p->setFont(QFont("SansSerif", 6, QFont::Normal));
            p->drawText(clip.x()+15+(i-1)*130, view.y()+280, "Lowest");
            tempInCel(p, f_lTemp.at(i), 15+(i-1)*130, 300, false, 12);
            p->setFont(QFont("SansSerif", 6, QFont::Normal));
            p->setPen(QColor(56, 193, 124));
            p->drawText(clip.x()+70+(i-1)*130, view.y()+280, "Highest");
            tempInCel(p, f_hTemp.at(i), 70+(i-1)*130, 300, false, 10);
            p->drawImage(QRect(clip.x()+20+(i-1)*130, view.y()+200, 50, 50), QImage(prefix+f_image.at(i)+".png"));
        }

        ///////////////// forecast information ///////////////////////
    }
    else
    {
        p->setPen(QColor(255, 18, 18, 127));
        p->drawText(clip.x()+40, view.y()+85, "--Error----");
        p->setPen(QColor(24, 200, 198, 127));
        p->drawText(clip.x()+40, view.y()+100, "Check network connection.");
        p->drawText(clip.x()+40, view.y()+115, "could not establish connection with google.");
        p->drawText(clip.x()+40, view.y()+130, "If problem persist, contact plexydesk dev team.");
        p->drawText(clip.x()+40, view.y()+145, "Online help available in IRC channel #plexydesk .");
        p->setPen(QColor(255, 18, 18, 127));
        p->drawText(clip.x()+40, view.y()+160, "----------");

    }


}
QString googleweatherWidget::getDay(QString day)
{
    if (day == "Sun")
        return "Sunday";
    else if (day == "Mon")
        return "Monday";
    else if (day == "Tue")
        return "Tuesday";
    else if (day == "Wed")
        return "Wednesday";
    else if (day == "Thu")
        return "Thursday";
    else if (day == "Fri")
        return "Friday";
    else if (day == "Sat")
        return "Saturday";
    else
        return "Cannot determine";
}
void googleweatherWidget::drawWidget()
{
    widgetBack = QImage (prefix + "background.png");
    w_ImageDock = QImage (prefix + "weatherDock.png");
}
//---------------------------------------------------------------------
void googleweatherWidget::drawItems()
{
    update ();
}
void googleweatherWidget::setCity_UI(QPainter *p)
{
}
void googleweatherWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *widget)
{
    QRectF r = e->exposedRect;
    p->setCompositionMode(QPainter::CompositionMode_Source);
    p->fillRect(rect(), Qt::transparent);
    p->drawImage (QRect(r.x(), r.y(), widgetBack.width(), widgetBack.height()), widgetBack);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setBackgroundMode(Qt::TransparentMode);
    p->save ();
    p->setRenderHint (QPainter::SmoothPixmapTransform);
    p->setClipRect(clip);
    p->restore();
    //setCity_UI(p);
    drawSet(p);
    initTimer = 1;
}
void googleweatherWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem *e, QWidget *)
{
    p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);
    p->setPen(QColor(15, 10, 220));
    p->setFont(QFont("Bitstream Charter", 12));
    initD = 0;
    if (DS_EXTF == 0 && initTimer == 1)
    {
        QTimer::singleShot(200, this, SLOT(data()));
        //data();
    }
    else
        DS_EXTF = 0;

}
}
