import QtQuick 2.14
import QtQuick.Window 2.14
// import com.fathom.hexagonpanel 1.0

Window {
    width: 800
    height: 800
    visible: true
    title: qsTr("Hello World")

//    PyHexagon
//    {
//        x: 0
//        y: 0
//        width: 100
//        height: 100
//    }

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
