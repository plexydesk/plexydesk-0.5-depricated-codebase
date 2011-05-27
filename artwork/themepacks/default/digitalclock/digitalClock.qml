/*******************************************************************************
* This file is part of PlexyDesk.
* Maintained by : Siraj Razick <siraj@kde.org>
* Authored By : Varuna Lekamwasam <vrlekamwasam@gmail.com>
*
* PlexyDesk is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* PlexyDesk is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

import Qt 4.7
import "images"

Rectangle {
    id: digitalClockBase
    width: 219
    height: 87
    color: "#00000000"
    Timer
    {
        interval: 100
        running: true
        repeat: true;
        onTriggered: setTime(hidden.text)
    }
    Text
    {
        id:hidden
        text: "12"
        visible: false
    }

    Text {
        id: hours
        x: 5
        y: 0
        color: "#000000"
        text: "13:59"
        font.bold: false
        font.family: "Bitstream Charter"
        style: Text.Normal
        anchors.verticalCenterOffset: 0
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.topMargin: 0
        font.pixelSize: 60
    }

    Text {
        id: seconds
        x: 181
        y: 38
        color: "#000000"
        text: ": 59"
        anchors.right: parent.right
        anchors.rightMargin: 9
        font.family: "Bitstream Charter"
        anchors.bottomMargin: 10
        anchors.leftMargin: 15
        anchors.bottom: hours.bottom
        anchors.left: hours.right
        style: Text.Normal
        font.bold: false
        font.pixelSize: 20
    }

    Text {
        id: ampm
        x: 160
        y: 9
        color: "#000000"
        text: "AM"
        anchors.rightMargin: 4
        anchors.topMargin: 9
        font.family: "Bitstream Charter"
        style: Text.Normal
        font.pixelSize: 30
        anchors.top: hours.top
        anchors.right: seconds.right
        Behavior on opacity {
            NumberAnimation{
                duration: 500
            }
        }
    }

    Text {
        id: date
        x: 15
        y: 65
        color: "#000000"
        text: "Wednesday, September 27"
        anchors.top: hours.bottom
        anchors.topMargin: -8
        font.family: "Bitstream Charter"
        anchors.left: hours.left
        anchors.leftMargin: 0
        font.bold: false
        style: Text.Normal
        font.pixelSize: 16
        Behavior on opacity {
            NumberAnimation{duration:1000}
        }
    }

    Text {
        id: timeFormatNotification
        x: 15
        y: 72
        color: "#000000"
        text: "Time format changed to "+hidden.text+" hour clock"
        anchors.left: date.left
        anchors.leftMargin: 0
        opacity: 0
        style: Text.Normal
        font.pixelSize: 10
        Behavior on opacity {
            NumberAnimation{duration: 4000}
        }
    }

    Rectangle {
        id: settingsParent
        x: 202
        y: 70
        width: 15
        height: 15
        color: "#5c5a5a"
        radius: 4
        clip: true
        Behavior on color{
            NumberAnimation{duration:200}
        }

        Image {
            id: settingsIcon
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "images/config.png"

            MouseArea {
                id: config
                anchors.fill: parent
                onClicked: changeConfig()
                opacity: 1
                hoverEnabled: true;
                onEntered: settingsParent.color = "#ffffff"
                onExited: settingsParent.color = "#5c5a5a"
            }
        }
    }


    states: [
        State {
            name: "configChanged"
            PropertyChanges {
                target: timeFormatNotification
                opacity: 1
            }

            PropertyChanges {
                target: date
                opacity: 0
            }
        }
    ]
    transitions: [
        Transition {
            from: ""
            to: "configChanged"
            reversible:  true
        }
    ]

    function setTime(type)
    {
        var dt = new Date();
        var hr = dt.getHours();
        var mn = dt.getMinutes();
        var sc = dt.getSeconds();
        if (mn < 10)
            mn = "0" + mn;
        if (hr < 10)
            hr = "0" + hr;
        if (sc < 10)
            sc = "0" + sc;
        var amorpm = "NA";
        if (type === "12")
        {
            if(hr > 12)
            {
                hr = hr - 12;
                amorpm = "PM";
                if (hr < 10)
                    hr = "0" + hr;
            }
            else if (hr === "12")
            {
                amorpm = "PM";
            }
            else
            {
                amorpm = "AM";
            }
        }
        ampm.text = amorpm;
        hours.text = hr + ":" + mn;
        seconds.text = ": " + sc;

        date.text = getDayString(dt.toDateString().substring(0,3))+", "+getMonthString(dt.getMonth())+" "+dt.getDate();
        setDisplay(type);
    }
    function changeConfig()
    {
        digitalClockBase.state = "configChanged"
        if (hidden.text === "12")
            hidden.text = "24";
        else
            hidden.text = "12";
    }
    function setDisplay(type)
    {
        if (timeFormatNotification.opacity === 1)
            digitalClockBase.state = "";
        if (type === "24")
            ampm.opacity = 0;
        else if (type === "12")
            ampm.opacity= 1;
    }
    function getDayString(day)
    {
        switch(day)
        {
        case "Sun": return "Sunday";
        case "Mon": return "Monday";
        case "Tue": return "Tuesday";
        case "Wed": return "Wednesday";
        case "Thu": return "Thursday";
        case "Fri": return "Friday";
        case "Sat": return "Saturday";
        }
    }
    function getMonthString(month)
    {
        switch(month)
        {
        case 0: return "January";
        case 1: return "February";
        case 2: return "March";
        case 3: return "April";
        case 4: return "May";
        case 5: return "June";
        case 6: return "July";
        case 7: return "August";
        case 8: return "September";
        case 9: return "Octorber";
        case 10:return "November";
        case 11:return "December";
        }
    }
}
