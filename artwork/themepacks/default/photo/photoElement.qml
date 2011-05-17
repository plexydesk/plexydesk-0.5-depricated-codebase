import Qt 4.7
Rectangle {
    width:200 
    height:176 
    color : "transparent"
    Image {
        source:"photoElement_images/background.png"
        id:background
        x:0 ; y:0
        width:200 
        height:176 
    }
    Image {
        parent : background
        source:"photoElement_images/pasted_layer.png"
        id:pasted_layer
        x:37 ; y:40
        width:120 
        height:75 
    }
}
