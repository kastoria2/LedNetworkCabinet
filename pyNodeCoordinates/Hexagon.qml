import QtQuick 2.0
import com.fathom.hexagonpanel 1.0

Item {
    id: rootItem

    property color outlineColor: "black"
    property color infillColor: "white"

    property int lineWidth: 2

    property HexagonModel hexagonModel
    property double toScreenScale

    width: hexagonModel.getDiameter() * toScreenScale
    height: hexagonModel.getHeight() * toScreenScale

    x: (hexagonModel.getX() * toScreenScale) - width/2
    y: (hexagonModel.getY() * toScreenScale) - height/2

    Canvas {
        anchors.fill: parent

        // vertices can have absolute coords.  Hexagon(x=100, y=50, radius=25).
        // Vertices are returned relative to that coordanites.
        // Those coordinates map into screen space relative to the current
        // pen position.  e.g. (125, 50) is vert[0] in world space but relative
        // to the hex being drawn it is (radius*screenScale, 0).
        function toHexagonScreenSpace(v) {
            return [
                (v[0]-hexagonModel.getX()) * toScreenScale,
                (v[1]-hexagonModel.getY()) * toScreenScale
            ];
        }

        onPaint: {
            var ctx = getContext("2d");

            ctx.lineWidth = lineWidth;
            ctx.strokeStyle = outlineColor;
            ctx.fillStyle = infillColor;

            ctx.translate(width/2, height/2);

            ctx.beginPath();

            var verts = hexagonModel.vertices;

            var currentPoint = toHexagonScreenSpace(verts[0]);
            ctx.moveTo(currentPoint[0], currentPoint[1])
            for(var i = 1; i < verts.length; i++)
            {
                currentPoint = toHexagonScreenSpace(verts[i]);
                ctx.lineTo(currentPoint[0], currentPoint[1]);
            }

            ctx.closePath();

            ctx.fill();
            ctx.stroke();
            ctx.translate(-width/2, -height/2);
        }
    }
}
