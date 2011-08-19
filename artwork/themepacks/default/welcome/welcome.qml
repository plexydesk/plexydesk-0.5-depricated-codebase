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
import "scripts/Scripts.js" as Scripts


Rectangle {

    id: rectangle1
    width: 460
    height: 260
    color: "#00000000"
    opacity: 1.0

    Image {
        id: background
        source: "image://plexydesk/qml_widgets_background"
        anchors.fill: parent
        z: -100
        asynchronous : true
        opacity: 0.35
        smooth: true
    }

    Image {
        id: background_content
        source: "image://plexydesk/qml_widgets_container_background"
        anchors.top: background.top
        anchors.topMargin: 20
        anchors.left: background.left
        anchors.leftMargin: 20
        z: -90
        width: background.width - (2*20)
        height: background.height - (2*20)
        opacity: 0.8
        asynchronous : true
        smooth: true
    }

    Text {
        id: gettingstarted
        anchors.top: background_content.top
        anchors.topMargin: 10
        anchors.left: background_content.left
        anchors.leftMargin: 20
        font.family: "Bitstream Charter"
        font.pixelSize: 40
        color: "white"
        text: "Getting Started"
        style: Text.Raised
        smooth: true
        opacity: 1.0

        Behavior on opacity {
            NumberAnimation{duration : 100}
        }
    }

    Text {
        id: plexydesk
        anchors.top: gettingstarted.bottom
        anchors.topMargin: 5
        anchors.left: gettingstarted.right
        anchors.leftMargin: -40
        color: "white"
        font.pixelSize: 30
        text: "PlexyDesk"
        style: Text.Raised
        smooth: true
        opacity: 1.0

        Behavior on opacity {
            NumberAnimation{duration : 100}
        }
    }

    Rectangle {
        id: container
        anchors.top: plexydesk.bottom
        anchors.topMargin: 20
        anchors.bottom: background_content.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: background_content.horizontalCenter
        width: background_content.width - 20
        color: "#00000000"
        opacity: 1.0

        Rectangle {
            id: item1
            parent: container
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: - (parent.width/4)
            width: 160
            height: 20
            color: "#00000000"
            radius: 2

            Text {
                id: entry
                color: "#ffffff"
                text: "Desktop"
                style: Text.Sunken
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            MouseArea {
                id: itemclick
                anchors.fill: parent
                hoverEnabled: true
                onEntered: Scripts.onEnteredDo(itemBack)
                onExited: Scripts.onExitedDo(itemBack)
                onClicked: Scripts.buttonClicked(rectangle1,"State1","Desktop",info_topic,info_desc)
            }

            Rectangle {
                id: itemBack
                color: "#5c5c5c"
                radius: 5
                z: -5
                opacity: 0.8
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation{duration : 100}
            }
        }

        Rectangle {
            id: item2
            width: 160
            height: 20
            color: "#00000000"
            radius: 2
            anchors.left: item1.left
            anchors.top: item1.bottom
            anchors.topMargin: 10

            Text {
                id: entry1
                color: "#ffffff"
                text: "Adding widgets"
                style: Text.Sunken
                font.pixelSize: 12
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: itemclick1
                anchors.fill: parent
                hoverEnabled: true
                onEntered: Scripts.onEnteredDo(itemBack1)
                onExited: Scripts.onExitedDo(itemBack1)
                onClicked: Scripts.buttonClicked(rectangle1,"State1","Adding Widgets",info_topic,info_desc)
            }

            Rectangle {
                id: itemBack1
                color: "#5c5c5c"
                radius: 5
                z: -5
                opacity: 0.8
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation{duration : 100}
            }
        }

        Rectangle {
            id: item3
            width: 160
            height: 20
            color: "#00000000"
            radius: 2
            anchors.left: item2.left
            anchors.top: item2.bottom
            anchors.topMargin: 10

            Text {
                id: entry2
                color: "#ffffff"
                text: "Removing Widgets"
                style: Text.Sunken
                font.pixelSize: 12
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: itemclick2
                anchors.fill: parent
                hoverEnabled: true
                onEntered: Scripts.onEnteredDo(itemBack2)
                onExited: Scripts.onExitedDo(itemBack2)
                onClicked: Scripts.buttonClicked(rectangle1,"State1","Removing Widgets",info_topic,info_desc)
            }

            Rectangle {
                id: itemBack2
                color: "#5c5c5c"
                radius: 5
                z: -5
                opacity: 0.8
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation{duration : 100}
            }
        }

        Rectangle {
            id: item4
            parent: container
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width/4
            width: 160
            height: 20
            color: "#00000000"
            radius: 2

            Text {
                id: entry3
                color: "#ffffff"
                text: "Changing Wallpaper"
                style: Text.Sunken
                font.pixelSize: 12
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: itemclick3
                anchors.fill: parent
                hoverEnabled: true
                onEntered: Scripts.onEnteredDo(itemBack3)
                onExited: Scripts.onExitedDo(itemBack3)
                onClicked: Scripts.buttonClicked(rectangle1,"State1","Changing Wallpaper",info_topic,info_desc)
            }

            Rectangle {
                id: itemBack3
                color: "#5c5c5c"
                radius: 5
                z: -5
                opacity: 0.8
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation{duration : 100}
            }
        }

        Rectangle {
            id: item5
            width: 160
            height: 20
            color: "#00000000"
            radius: 2
            anchors.left: item4.left
            anchors.top: item4.bottom
            anchors.topMargin: 10

            Text {
                id: entry4
                color: "#ffffff"
                text: "Development"
                style: Text.Sunken
                font.pixelSize: 12
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: itemclick4
                anchors.fill: parent
                hoverEnabled: true
                onEntered: Scripts.onEnteredDo(itemBack4)
                onExited: Scripts.onExitedDo(itemBack4)
                onClicked: Scripts.buttonClicked(rectangle1,"State1","Development",info_topic,info_desc)
            }

            Rectangle {
                id: itemBack4
                color: "#5c5c5c"
                radius: 5
                z: -5
                opacity: 0.8
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation{duration : 100}
            }
        }

        Rectangle {
            id: item6
            width: 160
            height: 20
            color: "#00000000"
            radius: 2
            anchors.left: item5.left
            anchors.top: item5.bottom
            anchors.topMargin: 10

            Text {
                id: entry5
                color: "#ffffff"
                text: "Wiki"
                style: Text.Sunken
                font.pixelSize: 12
                smooth: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: itemclick5
                anchors.fill: parent
                hoverEnabled: true
                onEntered: Scripts.onEnteredDo(itemBack5)
                onExited: Scripts.onExitedDo(itemBack5)
                onClicked: Scripts.buttonClicked(rectangle1,"State1","Wiki",info_topic,info_desc)
            }

            Rectangle {
                id: itemBack5
                color: "#5c5c5c"
                radius: 5
                z: -5
                opacity: 0.8
                anchors.fill: parent
            }

            Behavior on opacity {
                NumberAnimation{duration : 100}
            }
        }
    }

    Rectangle {
        id: information
        x: 0
        y: 0
        z: 100
        color: "#00000000"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        Rectangle {
            id: info_back
            anchors.fill: information
            anchors.margins: 30
            color: "#ffffff"
            radius: 10
            opacity: 0

            Behavior on opacity {
                NumberAnimation{duration: 200}
            }
        }

        Text {
            id: info_topic
            anchors.top: info_back.top
            anchors.left: info_back.left
            anchors.margins: 10
            width: info_back.width - 20
            height: 23
            color: "#ffffff"
            text: "info_topic"
            style: Text.Raised
            font.bold: true
            font.pixelSize: 20
            opacity: 0
            smooth: true

            Behavior on opacity {
                NumberAnimation{duration: 200}
            }
        }

        Text {
            id: info_desc
            anchors.top: info_topic.bottom
            anchors.left: info_back.left
            anchors.margins: 10
            width: info_back.width - 20
            color: "#ffffff"
            text: "info_desc"
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignJustify
            font.pixelSize: 12
            opacity: 0
            smooth: true

            onLinkActivated: Qt.openUrlExternally(link)

            Behavior on opacity {
                NumberAnimation{duration: 200}
            }
        }

        Image {
            id: backtohome
            anchors.top: info_back.top
            anchors.right: info_back.right
            anchors.margins: 10
            width: 25
            height: 23
            source: "image://plexydesk/back"
            asynchronous : true
            opacity: 0
            smooth: true

            Behavior on opacity {
                NumberAnimation{duration: 200}
            }

            MouseArea {
                id: backClick
                anchors.fill: parent
                opacity: 0
                hoverEnabled: true
                onEntered: {
                        backtohome.opacity = 1
                        backtohome.width = backtohome.width + 2
                        backtohome.height = backtohome.height + 2
                        }
                onExited:  {
                        backtohome.opacity = 0.8
                        backtohome.width = backtohome.width - 2
                        backtohome.height = backtohome.height - 2
                        }
                onClicked: Scripts.buttonClicked(rectangle1,"",info_topic.text,info_topic,info_desc)
            }
        }
    }

    states: [
        State {
            name: "State1"
            PropertyChanges {
                target: gettingstarted
                opacity: "0"
            }

            PropertyChanges {
                target: plexydesk
                opacity: "0"
            }

            PropertyChanges {
                target: item1
                opacity: "0"
            }

            PropertyChanges {
                target: item4
                opacity: "0"
            }

            PropertyChanges {
                target: itemclick1
            }

            PropertyChanges {
                target: item2
                opacity: 0
            }

            PropertyChanges {
                target: item3
                opacity: 0
            }

            PropertyChanges {
                target: item5
                opacity: 0
            }

            PropertyChanges {
                target: item6
                opacity: 0
            }

            PropertyChanges {
                target: info_back
                opacity: 0.2
            }

            PropertyChanges {
                target: info_desc
                opacity: 1
            }

            PropertyChanges {
                target: info_topic
                opacity: 1
            }

            PropertyChanges {
                target: information
                x: 0
                y: 0
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
            }

            PropertyChanges {
                target: backtohome
                opacity: 1
            }

            PropertyChanges {
                target: backClick
                opacity: 1
            }

            PropertyChanges {
                target: background
                opacity: 0.35
            }
        }
    ]

    transitions:[ Transition {
        from: ""
        to: "State1"
        SequentialAnimation{
            ParallelAnimation
            {
                NumberAnimation { targets: [gettingstarted,plexydesk]; properties: "opacity"; duration: 500 }
            }
            NumberAnimation { targets: [item1,item4]; properties: "opacity"; duration: 200 }
            NumberAnimation { targets: [item2,item5]; properties: "opacity"; duration: 200 }
            NumberAnimation { targets: [item3,item6]; properties: "opacity"; duration: 200 }
            NumberAnimation { target: info_back; property: "opacity"; to: 0.2; duration: 200 }
            NumberAnimation { target: info_topic; property: "opacity"; to: 1; duration: 200 }
            ParallelAnimation{
            NumberAnimation { target: info_desc; property: "opacity"; to: 1; duration: 200 }
            NumberAnimation { target: backtohome; property: "opacity"; to: 1; duration: 200 }
            }
        }
    },
        Transition {
                from: "State1"
                to: ""
                SequentialAnimation{
                    ParallelAnimation{
                    NumberAnimation { target: info_desc; property: "opacity"; to: 0; duration: 200 }
                    NumberAnimation { target: backtohome; property: "opacity"; to: 0; duration: 200 }
                    }
                    NumberAnimation { target: info_topic; property: "opacity"; to: 0; duration: 200 }
                    NumberAnimation { target: info_back; property: "opacity"; to: 0; duration: 200 }
                    ParallelAnimation
                    {
                        NumberAnimation { targets: [gettingstarted,plexydesk]; properties: "opacity"; duration: 500 }
                    }
                    NumberAnimation { targets: [item1,item4]; properties: "opacity"; duration: 200 }
                    NumberAnimation { targets: [item2,item5]; properties: "opacity"; duration: 200 }
                    NumberAnimation { targets: [item3,item6]; properties: "opacity"; duration: 200 }
                }
            }
    ]
}
