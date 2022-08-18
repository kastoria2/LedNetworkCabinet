import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    width: 800
    height: 800
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        id: border
        width: parent.width - 10
        height: parent.height - 10
        anchors.centerIn: parent
        color: "white"

        Hexpanel
        {
            width: parent.width
            height: parent.height
        }
    }
}
