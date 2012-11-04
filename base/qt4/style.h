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

#ifndef STYLE_H
#define STYLE_H

#include <plexy.h>

#include <QObject>
#include <QPainter>
#include <QRectF>

namespace PlexyDesk {

class StyleFeatures {

public:
    typedef enum {
        SF_None,
        SF_Raised,
        SF_MouseOver,
        SF_FrontView,
        SF_BackView
    } State;

    QRectF exposeRect;
    State state;

    //Line Edit specific
    int cursorLocation;
};

class Style : public QObject
{
    Q_OBJECT
public:
    //style enums
    typedef enum {
        CE_PushButton,
        CE_CheckBox,
        CE_Label,
        CE_ToolButton,
        CE_ToolBar,
        CE_LineEdit,
        CE_Seperator,
        CE_ListItem,
        CE_Frame
    } ControlElement;

    explicit Style(QObject *parent = 0);
    ~Style();

    virtual void paintControlElement(ControlElement element, const StyleFeatures &feature, QPainter *painter) = 0;
    virtual void paintControlElementText(ControlElement element, const StyleFeatures &feature, const QString &text, QPainter *painter) = 0;

Q_SIGNALS:
    void styleChanged();

};

}

#endif // STYLE_H
