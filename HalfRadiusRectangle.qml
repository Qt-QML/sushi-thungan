import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

Rectangle {
    property int bottomRadius: 15
    property int radiusWidth: 1
    property color rcColor: "#FF5722"

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        width: parent.width
        height: bottomRadius * 3
//        radius: bottomRadius

        border.color: rcColor
        border.width: radiusWidth
    }

    Rectangle {
        id: rectTop
        anchors.top: parent.top
        anchors.left: parent.left

        width: parent.width
        height: parent.height - bottomRadius

        border.color: rcColor
        border.width: radiusWidth
    }

    Rectangle {
        width: parent.width - radiusWidth*2
        x: radiusWidth
        anchors.bottom: rectTop.bottom
        height: radiusWidth
    }

    color: "transparent"
}
