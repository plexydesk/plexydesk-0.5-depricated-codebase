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


#include <desktopwidget.h>

#ifndef PLEXYDESK_QML_DESKTOP_WIDGET_H
#define PLEXYDESK_QML_DESKTOP_WIDGET_H

namespace PlexyDesk
{
class QmlDesktopWidget : public AbstractDesktopWidget
{
    Q_OBJECT

public:
    QmlDesktopWidget(const QRectF &rect, QWidget *embeddedWidget = 0, QDeclarativeItem *parent = 0);

    virtual ~QmlDesktopWidget();

    QDeclarativeEngine *engine() const;

    void setSourceUrl(const QUrl &url);

public Q_SLOTS:
    void onQuit();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    class PrivateQmlDesktopWidget;
    PrivateQmlDesktopWidget *const d;
};
} // namespace PlexyDesk
#endif //PLEXYDESK_QML_DESKTOP_WIDGET_H
