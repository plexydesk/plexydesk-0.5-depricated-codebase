/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Varuna Lekamwasam <vrlekamwasam@gmail.com>
*                 PhobosK <phobosk@kbfx.net>
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
    property alias typeIcon: iconImage.source
    id: icon
    width: parent.width
    height: parent.height
    anchors.horizontalCenter: parent.horizontalCenter
    color: "#00000000"

    Text {
        id:hidden
        visible: false
    }

    Image {
        id:iconImage
        anchors.horizontalCenter: icon.horizontalCenter
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenter: icon.verticalCenter
        anchors.verticalCenterOffset: 0
        width: icon.width - 20
        height: icon.height - 20
        opacity: 1.0
        smooth: true
        asynchronous : true
    }

    Text {
        id: fileNameText
        anchors.horizontalCenter: iconImage.horizontalCenter
        anchors.horizontalCenterOffset: 0
        anchors.verticalCenter: iconImage.verticalCenter
        anchors.verticalCenterOffset: (iconImage.height / 2) + 3
        z: 10
        color: "silver"
        text: Scripts.getFileName(filename)
        wrapMode: Text.WordWrap
        style: Text.Raised
        font.family: "Ubuntu"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        smooth: true
    }
}
