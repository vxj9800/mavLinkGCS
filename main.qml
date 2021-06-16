import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import myPackages.myDrone 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("My Ground Control")
    color: "gray"

    Item {
        id: mainWindow
        anchors.fill: parent

        MyDrone{
            id: drone
            rxPort: 14550
            txPort: 18570
        }

        Rectangle{
            id: topBar
            anchors.top: mainWindow.top
            width: mainWindow.width
            height: 64
            color: drone.hBeat ? "green" : "red"
            Text {
                id: connectionStatus
                anchors.right: topBar.right
                anchors.verticalCenter: topBar.verticalCenter
                font.pixelSize: topBar.height/4
                color: "white"
                text: drone.connectionStat
            }
            Button{
                id: arm
                anchors.left: topBar.left
                anchors.leftMargin: 5
                anchors.verticalCenter: topBar.verticalCenter
                onClicked: drone.armDrone()
                text: "Arm"
            }
//            Button{
//                id: manualMode
//                anchors.left: arm.right
//                anchors.leftMargin: 5
//                anchors.verticalCenter: topBar.verticalCenter
//                onClicked: drone.manualDrone()
//            }
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: topBar.bottom
            anchors.bottom: mainWindow.bottom
            id: headingBackground
            height: Math.min(parent.height, parent.width)
            width: Math.min(parent.height, parent.width)
            source: "images/heading/background.png"
            autoTransform: true
            fillMode: Image.PreserveAspectFit
            rotation: drone.heading < 36000 ? -drone.heading/100.0 : 0
            opacity: drone.heading < 36000 ? 1.0 : 0.2
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: topBar.bottom
            anchors.bottom: mainWindow.bottom
            id: headingNeedle
            height: Math.min(parent.height, parent.width)
            width: Math.min(parent.height, parent.width)
            source: "images/heading/needle.png"
            autoTransform: true
            fillMode: Image.PreserveAspectFit
            opacity: drone.heading < 36000 ? 1.0 : 0.2
        }
    }
}
