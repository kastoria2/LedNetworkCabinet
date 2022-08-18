import QtQuick 2.0
import com.fathom.hexagonpanel 1.0

Rectangle {
    id: hexpanel

    property HexagonPanelModel hexagonPanelModel

    property double widthScale: width / hexagonPanelModel.width()
    property double heightScale: height / hexagonPanelModel.height()

    color: "light gray"

    Item {
        // Raster offset so the (0,0) hexagon at (0,0) world space is on screen.

        // Raster space rendering the 'panel' using a coordinate space detailed in
        // the Python HexagonPanel object.
        // Both spaces have origin (0,0) at top left with
        // increasing x to right and y down.

        x: hexagonPanelModel.radius() * hexpanel.widthScale
        y: hexagonPanelModel.radius() * hexpanel.widthScale

        Repeater {
            id: rowRepeater
            model: hexpanel.hexagonPanelModel.rows()

            Repeater {
                id: columnRepater
                model: hexpanel.hexagonPanelModel.columns()

                property int rowIndex: index

                Hexagon {

                    outlineColor: "gray"
                    infillColor: "transparent"

                    toScreenScale: widthScale
                    hexagonModel: hexagonPanelModel.hexagonAtIndex(index, rowIndex)
                }
            }
        }

        Repeater {
            id: ledRepeater
            model: hexpanel.hexagonPanelModel.leds

            Rectangle {
                function rgbToQtColor(rgb) {

                    var red = (rgb & 0xff0000) >> 16;
                    var green = (rgb & 0x00ff00) >> 8;
                    var blue = rgb & 0x0000ff;

                    // console.debug(`rgb: ${rgb}, red: ${red}, green: ${green}, blue: ${blue}`);

                    return Qt.rgba(red / 256.0, green/256.0, blue/256.0, 1.0);
                }

                color: rgbToQtColor(modelData.finalColor)

                x: (modelData.position[0] * hexpanel.widthScale) - width/2
                y: (modelData.position[1] * hexpanel.widthScale) - height/2

                width: 10
                height: 10

                radius: width * 0.5
            }
        }
    }

    Timer {
        interval: 16

        running: true
        repeat: true

        onTriggered: {
            hexagonPanelModel.update();
        }
    }

    Connections {
        target: hexagonPanelModel
        function onLedsChanged() {
            // console.debug("leds changed.");
        }
    }

}
