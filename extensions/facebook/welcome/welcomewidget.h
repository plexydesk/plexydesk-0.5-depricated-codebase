/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
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

#ifndef PLEXY_WELCOME_WIDGET_H
#define PLEXY_WELCOME_WIDGET_H


#include <datainterface.h>
#include <pluginloader.h>

#include <desktopwidget.h>
#include <listview.h>

class QTimer;

class WelcomeWidget: public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    WelcomeWidget(const QRectF &rect, QWidget *widget = 0);
    virtual ~WelcomeWidget(); 
    void paintBackSide(QPainter *painter, const QRectF &rect);
    void paintViewSide(QPainter *painter, const QRectF &rect);

public Q_SLOTS:
    void data(QVariantMap&);
    void onDataReady();
    void fadeIn();
    void fadeOut();
    void showLogin();

Q_SIGNALS:
    void dataChanged();
    void newData(QVariantMap&);

private:
    QPixmap fromSvg(QRectF _rect, const QString& file);
    class Private;
    Private * const d;
};

#endif

