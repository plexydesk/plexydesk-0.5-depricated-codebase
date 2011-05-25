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

#ifndef PLEXY_GOOGLEWEATHER_WIDGET_H
#define PLEXY_GOOGLEWEATHER_WIDGET_H

#include <datainterface.h>
#include <pluginloader.h>
#include <desktopwidget.h>
#include <QTimer>
#include "socialqdbusplugindata.h"
namespace PlexyDesk
{
class googleweatherWidget : public DesktopWidget
{

    Q_OBJECT

public:
    googleweatherWidget(const QRectF &rect, QWidget *widget = 0);
    virtual ~googleweatherWidget();
    virtual void paintExtFace(QPainter *painter, const QStyleOptionGraphicsItem *e, QWidget *);
    void paintExtDockFace(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setPath(QString);
    void drawWidget();
public slots:
    void drawItems();
    void data();
private:
    void tempInCel(QPainter *p, QString temp, int x, int y, bool c, int size);
    QString getDay(QString);
    void drawSet(QPainter *p);
    void setCity_UI(QPainter *p);
    QRectF clip;
    QRectF view;
    int shade;
    int moveY;
    QImage widgetBack;
    QImage w_ImageDock;
    QVariantMap weatherdata;
    SocialQDBusPluginData *weather;
    QStringList weatherKeys;
    QStringList weatherValues;
    QString prefix;
    int initD, DS_EXTF, initTimer;
};

}
#endif

