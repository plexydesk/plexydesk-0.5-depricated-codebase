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
#include <controllerinterface.h>
#include <abstractdesktopwidget.h>

namespace PlexyDesk
{

class PLEXYDESKCORE_EXPORT AbstractDesktopView : public QGraphicsView
{
    Q_OBJECT

public:
    AbstractDesktopView(QGraphicsScene *scene = new QGraphicsScene(), QWidget *parent = 0);

    virtual ~AbstractDesktopView();

    virtual void enableOpenGL(bool);

    virtual bool setBackgroundController(const QString &controllerName);

    virtual void addController(const QString &controllerName);

    virtual QStringList currentControllers() const;

    virtual ControllerPtr controller(const QString &name);

    virtual void setControllerRect(const QString &controllerName, const QRectF &rect);

    virtual QSharedPointer<ControllerInterface> controllerByName(const QString &name);

    virtual void layout(const QRectF &rect) = 0;

    virtual void sessionDataForController(const QString &controllerName, const QString &key, const QString &value);

    virtual void restoreViewFromSession(const QString &sessionData);

    virtual void addWidgetToView(AbstractDesktopWidget *widget);

Q_SIGNALS:

    void closeApplication();

    void sessionUpdated(const QString &data);

public:
    virtual void onWidgetClosed(PlexyDesk::AbstractDesktopWidget *);

private:

    virtual void dropEvent(QDropEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);

    virtual void dragMoveEvent(QDragMoveEvent *event);

private:

    class PrivateAbstractDesktopView;

    PrivateAbstractDesktopView *const d;
};

} // namespace PlexyDesk
#endif
