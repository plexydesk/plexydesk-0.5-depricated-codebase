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
import Qt.labs.folderlistmodel 1.0
import FolderView 1.0
import "resources"
import "resources/images" as Images
import "resources/Scripts.js" as Scripts

Rectangle{
    id:backgroundHolder
    width: 528
    height: 454
    color: "#00000000"
    focus : false

    Image {
        id: background
        anchors.fill: parent
        parent : backgroundHolder
        asynchronous : true
        focus: false
        source: "image://plexydesk/qml_widget_background"
        opacity : 0.5
    }

    Text{
	id: currentDir
        x: 256
        y: 382
        color: "silver"
        text: Scripts.getFolderName(flm)
        font.pointSize: 12
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 53
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: rectangle1.horizontalCenter
        style: Text.Raised
        opacity: 1
    }
   
    Rectangle
    {
        id: rectangle1
        parent : backgroundHolder 
        width: 400
        height: 320
        clip: true
        x: 55
        y: 44
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        color: "#00000000"
        opacity:1
        radius: 5
        anchors.horizontalCenterOffset: -9
        anchors.horizontalCenter: parent.horizontalCenter
        GridView{
                id: gridView
                anchors.fill: parent
                cacheBuffer : 40
                snapMode : GridView.SnapToRow
                parent : rectangle1
                Component
                {
                        id:fileDelegate
                        Rectangle{
                            id: iconRect
                            width: 100
                            height: 100
                            Icon{filename: fileName}
                            opacity: 1.0
                            color: "#00000000"
                            MouseArea
                            {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: iconRect.opacity = 1
                                onExited:  iconRect.opacity = 0.8
                                onClicked: Scripts.goInto(fileName,flm,backgroundHolder,folderView)
                            }
                        }

                }
                model: flm
                delegate: fileDelegate
                focus: true
        }


    }

    FolderView 
    {
       id : folderView
    }

    FolderListModel
    {
        id: flm
        folder:folderView.home
        sortField : FolderListModel.Type
        nameFilters: ["*"]
    }


    Image {
        id: image1
        x: 52
        y: 382
        width: 25
        height: 23
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 49
        asynchronous : true
        parent : backgroundHolder
        source: "image://plexydesk/Back"
        opacity: 1
        MouseArea {
            id: mouse_area1
            anchors.fill: parent
            onClicked: Scripts.goUp(flm,backgroundHolder,background,rectangle1,currentDir,image1)
        }
    }
}
