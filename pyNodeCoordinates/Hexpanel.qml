import QtQuick 2.0

Rectangle {
    id: hexpanel

    property double physicalWidth: 2.0
    property double physicalHeight: 2.0

    property double hexagonRadius: 0.2

    property double widthScale: width / physicalWidth
    property double heightScale: height / physicalHeight

    property double columns: physicalWidth / hexagonRadius
    property double rows: (physicalHeight / hexagonRadius) * 2

    color: "light gray"

    Repeater {
        id: rowRepeater
        model: rows

        Repeater {
            id: columnRepater
            model: columns

            property int rowIndex: index

            Rectangle {
                color: "black"
                width: 5
                height: 5

                x: ((index * hexpanel.hexagonRadius) * widthScale) + (rowIndex%2)*(hexpanel.hexagonRadius/2)*widthScale
                y: ((rowIndex * hexpanel.hexagonRadius) * heightScale)/2
            }
        }
    }
}
