import Qt 4.7
import "Scripts.js" as Scripts
Rectangle {
    property alias filename: hidden.text
    property string type: Scripts.getType(filename)
    id: icon
    width: 100
    height: 100
    color: "#00000000"
    Text{
        id:hidden
        visible: false
    }

    Text {
        id: fileNameText
        x: 6
        y: 83
        color: "silver"
        text: Scripts.getFileName(filename)
        anchors.horizontalCenter: iconImage.horizontalCenter
        wrapMode: Text.WordWrap
        style: Text.Raised
        font.family: "Ubuntu"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
        z:10
    }
    Image
    {
        id:iconImage
        x: 10
        y: 3
        width: 80
        height: 80
        opacity: 1
        smooth: true
        fillMode: Image.PreserveAspectFit
        anchors.bottomMargin: 17
        source: Scripts.getTypeIcon(type)
    }

}
