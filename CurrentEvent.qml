import QtQuick 2.7
//import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import "controls" as Awesome

Item {
    id: root

    clip: true;
    property bool available: true
    property string clock : "--:--"
    property variant event: undefined

    function reloadCurrent(newEvent) {
        if (newEvent !== undefined) {
            console.log("new event ", newEvent.title)
            event = newEvent
            available = false

            titleLoader.sourceComponent = titleComponent
            eventDetailLoader.sourceComponent = eventDetailComponent
        }
    }

    function reloadAvailable() {
        titleLoader.sourceComponent = undefined;
        eventDetailLoader.sourceComponent = undefined;

        available = true
        event = undefined
    }

    Rectangle {
        id:rectRoot
        anchors.fill: parent
//        color: available? "#9BC832" : "#F77711"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ECEFF1" }
            GradientStop { position: 1.0; color: "white" }
        }
    }

    Item {
        anchors.fill: parent
//        GridLayout {
//            columns: 1
//            Rectangle {
//                id:rectTopContent
//                width: 1000
//                height: 50
//                Layout.rightMargin: 1
//                gradient: Gradient {
//                    GradientStop { position: 0.0; color: "#ECEFF1" }
//                    GradientStop { position: 1.0; color: "lightsteelblue" }
//                }

//                RowLayout {
//                    anchors.verticalCenter: parent.verticalCenter
//                    Label {
//                        Layout.leftMargin: 5
//                        text: "Bàn 1"
//                    }
//                }
//            }
//        }
        Rectangle {
            anchors.centerIn: parent
            width: parent.width
            height: width
            TableView {
                TableViewColumn {
                    role: "title"
                    title: "Title"
                    width: 100
                }
                TableViewColumn {
                    role: "author"
                    title: "Author"
                    width: 200
                }
                model: libraryModel
            }
        }
        ListModel {
            id: libraryModel
            ListElement {
                title: "A Masterpiece"
                author: "Gabriel"
            }
            ListElement {
                title: "Brilliance"
                author: "Jens"
            }
            ListElement {
                title: "Outstanding"
                author: "Frederik"
            }
        }
    }
}
