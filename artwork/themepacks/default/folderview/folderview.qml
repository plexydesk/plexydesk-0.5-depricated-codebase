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
    Image {
        id: background
        anchors.fill: parent
        source: "resources/images/background.png"
        z:-100
    }
    Rectangle {
    id: iconcontainer
    x: 46
    y: 34
    width: 420
    height: 386
    radius: 0
    anchors.verticalCenterOffset: 0
    anchors.horizontalCenterOffset: -8
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    opacity: 1
    color: "#00000000"
    Text{
	id: currentDir
        x: 210
        y: 347
        color: "silver"
        text: Scripts.getFolderName(flm)
        anchors.verticalCenterOffset: -10
        anchors.verticalCenter: rectangle3.verticalCenter
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        style: Text.Raised
        z: 400
        opacity: 1
    }
    MouseArea
    {
        id:msa1
        drag.axis: Drag.XandYAxis
        focus: true
    }
    Rectangle
    {
        id: rectangle1
        width: 400
        height: 320
        clip: true
        x: 10
        y: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        color: "#00000000"
        opacity:1
        radius: 5
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        z: 200
        GridView{
                id: gridView
                anchors.fill: parent
                Component
                {
                        id:fileDelegate
                        Rectangle{
                            id: iconRect
                            width: 100
                            height: 100
                            Icon{filename: fileName}
                            opacity: 0.8
                            color: "#00000000"
                            MouseArea
                            {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: iconRect.opacity = 1
                                onExited:  iconRect.opacity = 0.8
                                onClicked: Scripts.goInto(fileName,flm,backgroundHolder,imageViewer,imageText)
                            }
                        }

                }
                model: flm
                delegate: fileDelegate
                focus: true
                Behavior on opacity{
                    NumberAnimation{duration: 500}
                }
        }


    }
    FolderView{id:folderView}
    FolderListModel
    {
        id: flm
        folder: folderView.home
        nameFilters: ["*"]
    }


    Image {
        id: image1
        x: 10
        y: 349
        width: 25
        height: 23
        anchors.verticalCenterOffset: -5
        anchors.verticalCenter: rectangle3.verticalCenter
        source: "resources/images/Back.png"
        z: 400
        opacity: 1
        MouseArea {
            id: mouse_area1
            anchors.fill: parent
            onClicked: Scripts.goUp(flm)

        }
    }
    Image {
        id: imageViewer
        width: 0
        height: 0
        y:0
        visible: false
        anchors.horizontalCenter: rectangle1.horizontalCenter
        anchors.verticalCenter: rectangle1.verticalCenter
        clip: false
        smooth: true
        z:1
        fillMode: Image.PreserveAspectFit
        source: "resources/images/qt.png"
        Behavior on opacity{
            NumberAnimation{duration: 500}
        }

        MouseArea {
            id: viewImageClickArea
            opacity: 0
            anchors.fill: parent
            onClicked: Scripts.setOpacityBug(backgroundHolder,iconcontainer,rectangle1,rectangle2,currentDir,image1)
            visible: false
        }
    }
    Rectangle {
        id: imageControls
        x: 376
        y: 353
        width: 34
        height: 16
        color: "#3a3a3a"
        anchors.verticalCenterOffset: -5
        anchors.verticalCenter: rectangle3.verticalCenter
        opacity: 1
        visible: false
        Behavior on opacity{
            NumberAnimation{duration: 500}
        }

        Image {
            id: imageRotateLeft
            x:0
            y:0
            width: 16
            height: 16
            source: "resources/images/CCW.png"
            opacity: 1
            MouseArea
            {
                anchors.fill: parent
                onClicked: imageViewer.rotation = imageViewer.rotation - 90
            }
        }

        Image {
            id: imageRotateRight
            x:17
            y:0
            width: 16
            height: 16
            source: "resources/images/CW.png"
            opacity: 1
            MouseArea
            {
                anchors.fill: parent
                onClicked: imageViewer.rotation = imageViewer.rotation + 90
            }
        }
    }

    Text{
        id: imageText
        x: 210
        y: 10
        anchors.horizontalCenter: parent.horizontalCenter
        color: "silver"
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 10
        style: Text.Raised
        z:399
    }
    Rectangle {
        id: rectangle2
        color: "#000000"
        radius: 0
        visible: false
        opacity: 0.8
        anchors.fill: parent
        z:-1
    }

    Rectangle {
        id: rectangle3
        x: 0
        y: 340
        width: 420
        height: 41
        color: "#161515"
        radius: 0
        visible: false
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#222121"
            }

            GradientStop {
                position: 0.28
                color: "#141414"
            }

            GradientStop {
                position: 1
                color: "#000000"
            }
        }
        border.width: 2
        border.color: "#000000"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        opacity: 0.9
        z:-5
    }
    }
    states: [
        State {
            name: "viewImage"

            PropertyChanges {
                target: imageViewer
                x: 60
                y: 19
                width: 300
                height: 300
                anchors.verticalCenterOffset: 2
                anchors.horizontalCenterOffset: 0
                visible: true
                z: 199
            }

            PropertyChanges {
                target: viewImageClickArea
                focus: true
                opacity: 1
                visible: true
            }
            PropertyChanges {
                target: gridView
                focus : false
                z:1
                opacity: 0.1
            }

            PropertyChanges {
                target: imageText
                x: 196
                y: 1
                text: "test"
                anchors.horizontalCenterOffset: 0
                anchors.topMargin: 1
                anchors.bottomMargin: 10
            }

            PropertyChanges {
                target: imageControls
                x: 375
                y: 333
                opacity: 1
                visible: true
            }

            PropertyChanges {
                target: rectangle1
                x: 10
                y: 9
                anchors.horizontalCenterOffset: 0
            }

        }
    ]
}
