import Qt 4.7

Rectangle {
    width: 400
    height:176 * 2
    color : "transparent"
    Image {
        source:"image://plexydesk/photo_frame_background"
        id:background
        x:0 ; y:0
        width:200 * 2
        height:176 * 2
    }
    Image {
        source: plexydeskconfig.photo
        id:pasted_layer
        x:33*2 ; y:34*2
        width:128 * 2
        height:84 * 2
    }
}
