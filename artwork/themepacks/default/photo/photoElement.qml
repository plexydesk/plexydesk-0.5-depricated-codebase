import Qt 4.7

Rectangle {

    width:200 * 2
    height:176 * 2
    color : "transparent"

    Image {
        width:200 * 2 
        height:176 * 2
        source:"image://plexydesk/qml_widget_background"
        id:background
        x:0 ; y:0
        opacity : 0.5
    }

    Image {
        source : "http://www.wallpaperstop.com/wallpapers/baby-wallpapers/cute-baby-wallpaper-240x150-0912045.jpg"
        id:pasted_layer
        x:37 * 2 ; y:40 * 2 
        width:120 * 2
        height:75 * 2
        opacity: 0.0
        smooth : true

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
