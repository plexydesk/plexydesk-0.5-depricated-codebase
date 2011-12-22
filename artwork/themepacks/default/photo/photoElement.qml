import Qt 4.7

Item {
    id : main
    width: 400
    height: 400

    Image {
        id:background
        width : 400
        height : 400
        source: "image://plexydesk/qml_widgets_background"
        x:0 ; y:0
        opacity : 0.9
        asynchronous: true
        fillMode : Image.PreserveAspectFit

//----------------- Button : setWallpaper  --------------- 
        Rectangle {
              opacity : 1
              id : button1
              x : 20
              y : 280
              width : 160
              height : 20
              radius : 2
              color : "black"
              Text {
                 id : button_1_label_text
                 color : "white"
                 text : "Set as Wallpaper"
                 anchors.horizontalCenter: parent.horizontalCenter
                 anchors.verticalCenter: parent.verticalCenter
                 font.pixelSize: 12
                 style: Text.Sunken
                 }
          MouseArea {
                id : button_1_clicked
                anchors.fill: parent
                hoverEnabled: true
   
                onEntered : {
                               button_1_label_text.opacity = 0.8
                               button1.color = "white"
                               button_1_label_text.color = "black"
                            }
                onExited :  {
                               button_1_label_text.opacity = 1.0
                               button1.color = "black"
                               button_1_label_text.color = "white"
                            }

               onClicked : {
                                plexydeskconfig.wallpaper = PhotoSource.imageSource;
                                main.parent.visible = false;
                           }
        }
        Behavior on opacity {
                 NumberAnimation { duration : 100}
        }
    }
}

//----------------- Button : setAsPhoto --------------- 

        Rectangle {
              opacity : 1
              id : button2
              x : 222
              y : 280
              width : 160
              height : 20
              radius : 2
              color : "black"
              Text {
                 id : button_2_label_text
                 color : "white"
                 text : "Set as Photo Frame"
                 anchors.horizontalCenter: parent.horizontalCenter
                 anchors.verticalCenter: parent.verticalCenter
                 font.pixelSize: 12
                 style: Text.Sunken
                 }
          MouseArea {
                id : button_2_clicked
                anchors.fill: parent
                hoverEnabled: true
   
                onEntered : {
                               button_2_label_text.opacity = 0.8
                               button2.color = "white"
                               button_2_label_text.color = "black"
                            }
                onExited :  {
                               button_2_label_text.opacity = 1.0
                               button2.color = "black"
                               button_2_label_text.color = "white"
                            }
                onClicked : {
                                plexydeskconfig.photo = PhotoSource.imageSource;
                                main.parent.visible = false;
                           }

        }
        Behavior on opacity {
                 NumberAnimation { duration : 100}
        }
    }

  Rectangle {
        id : frame
        x:73 ; y: 99
        width : 244
        height : 154
        color : "white";
        border.width : 2;
        border.color : "white"
   Image {

        source : PhotoSource.imageSource
        id:pasted_layer
        x: 0 ; y: 2
        width: 240
        height: 150
        opacity: 0.0
        smooth : true
        asynchronous: true
        anchors.horizontalCenter: frame.horizontalCenter

        Behavior on opacity {
            NumberAnimation { from : 0.0 ; to: 1.0  ; duration: 1000 }
        }

        onStatusChanged: if (pasted_layer.status == Image.Ready) pasted_layer.state = "loaded"

         states: [
             State {
                 name: "loaded"
                 PropertyChanges { target: pasted_layer; opacity : 1.0 }
              }
        ]

    }
}
}
