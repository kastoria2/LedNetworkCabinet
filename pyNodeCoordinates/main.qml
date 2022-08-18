import QtQuick 2.14
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import QtQuick.Window 2.14

Window {
    width: 500
    height: 800
    visible: true
    title: qsTr("Hello World")

    ColumnLayout {
        spacing: 2

        anchors.fill: parent

        Button {
            text: "Generate firmware definitions"

            Layout.fillWidth: true

            height: 30

            onClicked: {
                var defs = GlobalHexagonPanelModel.generateFirmwareDefinitions();
                firmwareTextArea.text = defs;
                firmwareTextArea.selectAll();
            }
        }

        ScrollView
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 150

            TextArea {
                id: firmwareTextArea
                Layout.fillWidth: true
            }

        }

        Rectangle {
            id: border
            //width: parent.width - 10
            //height: parent.height - 10

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter

            color: "white"

            Hexpanel
            {
                width: parent.width
                height: parent.height

                hexagonPanelModel: GlobalHexagonPanelModel
            }
        }
    }
}
