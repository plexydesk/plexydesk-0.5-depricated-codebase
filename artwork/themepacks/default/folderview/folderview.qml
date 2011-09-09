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
import PlexyDesk.FolderListModel 1.0
import "resources"
import "resources/Scripts.js" as Scripts


Rectangle{
    id:backgroundHolder
    width: 500
    height: 400
    color: "#00000000"
    focus : false
    opacity: 1.0

    Image {
        id: background
        source: "image://plexydesk/qml_widgets_background"
        anchors.fill: parent
        z: -100
        asynchronous : true
        smooth : true
        opacity : 0.35
        focus: false
    }

    Image {
        id: background_folderview_content
        source: "image://plexydesk/qml_widgets_container_background"
        width: parent.width - (2 * 20)
        height: parent.height - background_currentDir.height - (3 * 20)
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        z: -90
        asynchronous : true
        focus: false
        opacity : 0.8
        smooth : true
    }

    Rectangle {
        id: folderview_content
        width: background_folderview_content.width - 20
        height: background_folderview_content.height - 20
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: background_folderview_content.horizontalCenter
        anchors.top: background_folderview_content.top
        anchors.topMargin: 5
        clip: true
        color: "#00000000"
        opacity: 1.0

        GridView {
            id: gridView
            parent : folderview_content
            anchors.fill: parent
            anchors.leftMargin: ( parent.width - (parseInt(parent.width/100) * 100) )/2
            cellWidth : 100
            cellHeight : 100
            cacheBuffer : 1000
            snapMode : GridView.SnapToRow
            model: flm
            delegate: fileDelegate
            focus: true

            Component {
                id:fileDelegate

                Rectangle {
                    id: iconRect
                    width: 100
                    height: 100

                    Icon {
                        filename: fileName
                        /* The one below is slow but accurate */
                        //typeIcon: fileMimeIcon
                        /* The one below is very fast but not so accurate */
                        typeIcon: fileTypeIcon
                    }

                    opacity: 0.8
                    color: "#00000000"

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: iconRect.opacity = 1.0
                        onExited:  iconRect.opacity = 0.8
                        onClicked: Scripts.goInto(fileName,flm,backgroundHolder)
                    }
                }
            }
        }
    }

    FolderListModel {
        id: flm
        sortField : FolderListModel.Type
        folder : DirSource.dirSource
        nameFilters: ["*"]
    }

    Image {
        id: background_currentDir
        source: "image://plexydesk/qml_widgets_container_background"
        height: background_folderview_content.anchors.topMargin
        width: background_folderview_content.width * (4/6)
        anchors.top: background_folderview_content.bottom
        anchors.topMargin: background_folderview_content.anchors.topMargin - 5
        anchors.right: background_folderview_content.right
        anchors.rightMargin: background_folderview_content.anchors.topMargin / 2
        asynchronous : true
        focus: false
        opacity : 0.8
        smooth : true
      }

    Text {
        id: currentDir
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: background_currentDir.horizontalCenter
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: background_currentDir.verticalCenter
        color: "white"
        text: Scripts.getFolderName(flm)
        font.pointSize: 12
        style: Text.Raised
        smooth: true
        opacity: 1.0
    }

    Image {
        id: background_button_back
        source: "image://plexydesk/qml_widgets_container_background"
        width: background_folderview_content.width * (1/6)
        height: background_folderview_content.anchors.topMargin
        anchors.top: background_folderview_content.bottom
        anchors.topMargin: background_folderview_content.anchors.topMargin - 5
        anchors.left: background_folderview_content.left
        anchors.leftMargin: background_folderview_content.anchors.topMargin / 2
        asynchronous : true
        focus: false
        opacity : 0.8
        smooth : true
    }

    Image {
        id: button_back
        source: "image://plexydesk/back"
        width: 25
        height: 23
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: background_button_back.horizontalCenter
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: background_button_back.verticalCenter
        asynchronous : true
        opacity: 1.0
        smooth: true

        MouseArea {
            id: mouse_area1
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                    button_back.opacity = 1
                    button_back.width = button_back.width + 2
                    button_back.height = button_back.height + 2
                    }
            onExited: {
                    button_back.opacity = 0.8
                    button_back.width = button_back.width - 2
                    button_back.height = button_back.height - 2
                    }
            onClicked: Scripts.goUp(flm,backgroundHolder,background,folderview_content,currentDir,button_back)
        }
    }
}
