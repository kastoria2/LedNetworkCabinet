import QtQuick 2.0
import com.fathom.hexagonpanel 1.0

Rectangle {
    id: hexpanel

    property HexagonPanelModel hexagonPanelModel

    property double widthScale: width / hexagonPanelModel.width()
    property double heightScale: height / hexagonPanelModel.height()

    color: "light gray"

    Repeater {
        id: rowRepeater
        model: hexpanel.hexagonPanelModel.rows()

        Repeater {
            id: columnRepater
            model: hexpanel.hexagonPanelModel.columns()

            property int rowIndex: index

            Hexagon {

                outlineColor: "black"
                infillColor: "light blue"

                toScreenScale: widthScale
                hexagonModel: hexagonPanelModel.hexagonAtIndex(index, rowIndex)
            }
        }
    }
}
