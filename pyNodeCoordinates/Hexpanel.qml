import QtQuick 2.0

Rectangle {
    id: hexpanel

    property double physicalWidth: 2.0
    property double physicalHeight: 2.0

    property double hexagonRadius: 0.2

    property double widthScale: width / physicalWidth
    property double heightScale: height / physicalHeight

    property double columns: physicalWidth / (hexagonRadius * 2)
    property double rows: physicalHeight / (hexagonRadius * 2) * 2

    color: "light gray"

    Text {
        text: `${widthScale}, ${heightScale}`
    }

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

                x: ((index * hexpanel.hexagonRadius * 3) * widthScale) + (rowIndex%2)*(hexpanel.hexagonRadius * 1.5)*widthScale
                y: (rowIndex * height/2)
            }
        }
    }
}
