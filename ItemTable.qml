import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {

    anchors.fill: parent

    property string btnIcon;
    property int tableRadius: 0
    property int tableMargin: 10
    property real itmOpacity: (ispressed ? 0.3 : 0.)

    property var tabColor: "green"

    // table tabstatus
    // 1: available
    // 2: have customer
    // 3: locked

    anchors.margins: tableMargin
    radius: tableRadius
    color: "transparent"

    signal clicked(string id);

    Rectangle {
        anchors.fill: parent
        anchors.margins: tableMargin
        radius: tableRadius
        color: "transparent"

        Column {
            spacing: 10
            anchors.fill: parent

            Label {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                text: table_name
                font.pointSize: 18
                color: tabColor
            }

            Rectangle {
                width: parent.width
                height: parent.height-100
                color: tabColor
                anchors.margins: 20

                Image {
                    visible: tabstatus == 2
                    x: 20
                    y: 20
                    width: parent.width-40
                    height: parent.height-40
                    source: "qrc:/images/icons/tableused.png"
                }
            }
        }
    }

    // click/pressed marker
    Rectangle {
        anchors.fill: parent
        radius: tableRadius
        color: ispressed ? "blue" : "transparent"
        opacity: itmOpacity
    }
}
