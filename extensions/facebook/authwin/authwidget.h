/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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

#ifndef PLEXY_FB_WIDGET_H
#define PLEXY_FB_WIDGET_H
#include <datainterface.h>
#include <pluginloader.h>
#include <desktopwidget.h>
#include <qwebviewitem.h>

class QTimer;
namespace PlexyDesk{

class AuthWidget: public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    AuthWidget(const QRectF &rect, QWidget *widget = 0);
    virtual ~AuthWidget();
    void readConfig(QString &user, QString &pass);
    void paintExtFace(QPainter *painter, 
            const QStyleOptionGraphicsItem *item, QWidget *widget);
    void paintExtDockFace(QPainter *painter,
            const QStyleOptionGraphicsItem *item, QWidget *widget);
public Q_SLOTS:
    void data(QVariantMap&);
    void onDataReady();
private:
      QWebViewItem *mView;
};
#endif
}
