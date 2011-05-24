import Qt 4.7
import Qt.labs.folderlistmodel 1.0
import "resources"
import "resources/images" as Images
import "resources/Scripts.js" as Scripts

Rectangle {
    id: iconcontainer
    //y:8
    width: 420
    height: 370
    radius: 10
    border.width: 2
    border.color: "#000000"
    opacity: 1
    color: "#00000000"
    Text{
	id: currentDir
        x: 120
        y: 7
        color: "#d5d5d5"
        text: Scripts.getFolderName(flm)
        anchors.horizontalCenter: parent.horizontalCenter
        style: Text.Raised
        z: 400
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
        y: 30
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        color: "#00000000"
        radius: 5
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
                                onClicked: Scripts.goInto(fileName,flm,iconcontainer,imageViewer,imageText)
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
    FolderListModel
    {
        id: flm
        folder: "/"
        nameFilters: ["*"]
    }


    Image {
        id: image1
        x: 8
        y: 5
        width: 25
        height: 23
        source: "resources/images/Back.png"
        z: 400
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
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
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
            anchors.fill: parent
            onClicked: iconcontainer.state = ""
            opacity: 0
        }
    }
    Rectangle {
        id: imageControls
        x: 346
        y: 319
        width: 34
        height: 16
        color: "#3a3a3a"
        opacity: 0.4
        visible: false
        Behavior on visible{
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
        anchors.horizontalCenter: imageViewer.horizontalCenter
        anchors.bottom: imageViewer.bottom
        anchors.bottomMargin: -5
        color: "silver"
        style: Text.Raised
        z:300
    }

    Rectangle {
        id: rectangle2
        color: "#000000"
        radius: 10
        opacity: 0.5
        anchors.fill: parent
        z:-1
    }

    Rectangle {
        id: rectangle3
        x: 0
        y: 0
        width: 420
        height: 30
        color: "#1e1b1b"
        radius: 10
        opacity: 0.8
        z:-1
    }
    states: [
        State {
            name: "viewImage"

            PropertyChanges {
                target: imageViewer
                width: 300
                height: 300
                visible: true
                z: 199
            }

            PropertyChanges {
                target: viewImageClickArea
                focus: true
                opacity: 1
            }
            PropertyChanges {
                target: gridView
                focus : false
                z:1
                opacity: 0.1
            }

            PropertyChanges {
                target: imageText
                text: "test"
                anchors.bottomMargin: 10
            }

            PropertyChanges {
                target: imageControls
                x: 375
                y: 333

                visible: true
            }

        }
    ]
}
