/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Varuna Lekamwasam <vrlekamwasam@gmail.com>
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

import Qt 4.7
import "Scripts.js" as Scripts
Rectangle {
    property alias filename: hidden.text
    property string type: Scripts.getType(filename)
    id: icon
    width: 100
    height: 100
    color: "#00000000"
    Text{
        id:hidden
        visible: false
    }

    Text {
        id: fileNameText
        x: 6
        y: 83
        color: "silver"
        text: Scripts.getFileName(filename)
        anchors.horizontalCenter: iconImage.horizontalCenter
        wrapMode: Text.WordWrap
        style: Text.Raised
        font.family: "Ubuntu"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        z:10
    }
    Image
    {
        id:iconImage
        x: 10
        y: 3
        width: 80
        height: 80
        opacity: 1
        smooth: true
        asynchronous : true
        fillMode: Image.PreserveAspectFit
        anchors.bottomMargin: 17
        source: Scripts.getTypeIcon(type)
    }

}
