import QtQuick 2.0
import com.fathom.hexagonpanel 1.0

Rectangle {
    id: hexpanel

    property double physicalWidth: (100*4) + (50 * 4.5)
    property double physicalHeight: (86.6 * 8) + (86.6)

    property double hexagonRadius: 50

    property double widthScale: width / physicalWidth
    property double heightScale: height / physicalHeight

    property double columns: physicalWidth / (hexagonRadius * 2)
    property double rows: physicalHeight / (hexagonRadius * 2) * 2

    property HexagonPanelModel hexagonPanelModel

    color: "light gray"

    Repeater {
        id: rowRepeater
        model: rows

        Repeater {
            id: columnRepater
            model: columns

            property int rowIndex: index

            Hexagon {
                color: "black"
                radius: hexpanel.hexagonRadius * widthScale

                x: ((index * hexpanel.hexagonRadius * 3) * widthScale) + ((rowIndex+1)%2)*(hexpanel.hexagonRadius * 1.5)*widthScale
                y: (rowIndex * height/2)
            }
        }
    }
}
