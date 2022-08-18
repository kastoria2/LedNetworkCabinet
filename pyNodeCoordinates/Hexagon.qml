import QtQuick 2.0

Item {
    property color color
    property double radius

    width: radius * 2
    height: width * (1/1.1547005)

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
            ctx.strokeStyle = "blue";
            ctx.fillStyle = "green";

            ctx.translate(width/2, height/2);

            ctx.beginPath();

            var currentPoint = [radius, 0];
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
