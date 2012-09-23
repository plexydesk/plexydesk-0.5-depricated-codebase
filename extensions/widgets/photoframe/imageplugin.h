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
#ifndef PLEXY_WIDGET_IMAGEPILE_H
#define PLEXY_WIDGET_IMAGEPILE_H

#include <plexy.h>

#include <abstractplugininterface.h>
#include <controllerinterface.h>

#include <themepackloader.h>
#include "photowidget.h"
#include "imageitem.h"

class PhotoFrameController : public PlexyDesk::ControllerInterface
{
    Q_OBJECT

public:
    PhotoFrameController(QObject *object = 0);
    virtual ~PhotoFrameController();

    virtual QGraphicsItem *defaultView();

    void handleDropEvent(PlexyDesk::AbstractDesktopWidget *widget, QDropEvent *event);

private:
    PhotoWidget *mFrameParentitem;
    QString mImageSource;
};

#endif
