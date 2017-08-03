import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Item {
    width: listViewEvents.width
    height: detailRow.height + 40    


    Rectangle

        {
            id: background
            anchors.fill: parent
            color: "white"
            height: 40
            width: 40
            Rectangle {
                height: 1
                anchors.bottom: parent.bottom
                width: parent.width
                color: "#D6D6D6"
            }


                Label {
                    text: (new Date(Date.parse(startEvent))).toLocaleTimeString(Qt.locale(), "HH:mm");
                    font.family: montserrat.name
                    font.pointSize: 18
                    font.italic: true
                    color: "#a5a6a7"
                }

        }



    MouseArea {
        anchors.fill: parent
        onClicked: {
            mainPage.starttime = startEvent.replace(".0", "")
            mainPage.endtime = endEvent.replace(".0", "")
            console.log("mainPage.url: " + mainPage.url);
            qrimage.setImage("image://colors/"+ mainPage.url + "?date=" + startEvent.replace(".0","").replace(" ","+") + "&room_id=" + mainPage.roomId)
            qrdialog.open();
        }
    }
}
