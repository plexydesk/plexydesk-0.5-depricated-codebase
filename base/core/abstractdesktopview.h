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

#ifndef PLEXY_ABSTRACT_DESKTOP_VIEW_H
#define PLEXY_ABSTRACT_DESKTOP_VIEW_H

#include <plexy.h>
#include <widgetplugin.h>
#include <abstractdesktopwidget.h>

namespace PlexyDesk
{

class AbstractDesktopView : public QGraphicsView
{
    Q_OBJECT

public:
    AbstractDesktopView(QGraphicsScene *scene = new QGraphicsScene(),
            QWidget *parent = 0) : QGraphicsView (scene, parent) {}

    virtual void addCoreExtension(const QString &name) = 0;

    virtual void addExtension(const QString &name,
                              const QString &layer = QLatin1String("Widgets"),
                              const QPoint &pos = QPoint(0, 0),
                              PlexyDesk::AbstractDesktopWidget::State state =
            PlexyDesk::AbstractDesktopWidget::DOCKED) = 0;

    virtual void enableOpenGL(bool);
    virtual void showLayer(const QString &name) = 0;

Q_SIGNALS:
    void closeApplication();
};

} // namespace PlexyDesk
#endif
