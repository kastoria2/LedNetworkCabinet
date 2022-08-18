import QtQuick 2.0
import com.fathom.hexagonpanel 1.0

Item {
    id: rootItem

    property color outlineColor: "black"
    property color infillColor: "white"

    property HexagonModel hexagonModel
    property double toScreenScale

    width: hexagonModel.getDiameter() * toScreenScale
    height: hexagonModel.getHeight() * toScreenScale

    x: hexagonModel.getX() * toScreenScale
    y: hexagonModel.getY() * toScreenScale

    Canvas {
        anchors.fill: parent

        function rotateVector2d(v, radians) {
            return [
                v[0] * Math.cos(radians) - v[1] * Math.sin(radians),
                v[0] * Math.sin(radians) + v[1] * Math.cos(radians)
                    ];
        }

        onPaint: {
            var ctx = getContext("2d");

            ctx.lineWidth = 4;
            ctx.strokeStyle = outlineColor;
            ctx.fillStyle = infillColor;

            ctx.translate(width/2, height/2);

            ctx.beginPath();

            var currentPoint = [hexagonModel.getRadius() * toScreenScale, 0];
            ctx.moveTo(currentPoint[0], currentPoint[1]);

            for(var i = 0; i < 6; i++)
            {
                currentPoint = rotateVector2d(currentPoint, 60 * Math.PI/180);
                ctx.lineTo(currentPoint[0], currentPoint[1]);
            }

            ctx.closePath();

            ctx.fill();
            ctx.stroke();
            ctx.translate(-width/2, -height/2);
        }
    }
}
