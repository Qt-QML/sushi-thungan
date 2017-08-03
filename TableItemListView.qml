import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Material.impl 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Private 1.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls.Universal 2.0
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4
import Qt.labs.platform 1.0

import com.vng.gbc.smr 1.0

Rectangle {
    id: rectContentListTable
    Component {
       id: contactDelegate
       Item {
           id: item
           width: gridContentListTable.cellWidth - 10
           height: gridContentListTable.cellHeight - 30
           anchors.margins: 5
           Column {
               anchors.fill: parent
               HalfRadiusRectangle {
                   id: itemInContentListTable
                   parent: gridContentListTable
                   x: item.x
                   y: item.y
                   width: item.width
                   height: item.height
                   rcColor: item.GridView.isCurrentItem? "#FF5722":"#E2E6E7"

                   Column {
                        anchors.fill: parent
                        Rectangle {
                            color: item.GridView.isCurrentItem? "#FF5722":"green"
                            width: item.width
                            height: item.height
                            Image {
                                width: item.width
                                height: item.height
                                source: "qrc:/images/icons/tableused.png"
                            }
                        }
                        Label {
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            text: table_name
                            font.pointSize: 18
                            color: item.GridView.isCurrentItem? "#FF5722":"green"
                        }
                   }
                   MouseArea{
                       anchors.fill: parent
                       onClicked: {
                           gridContentListTable.currentIndex = index
                           tableNumber = table_id
                           current_table_id = table_id
                           console.log("current_table_id: " + current_table_id)
                           MainController.getOrder(tableNumber)
                       }
                   }
               }
           }
       }
    }

    GridView {
        id: gridContentListTable
        anchors.fill: parent
        clip: true
        cellWidth: gridContentListTable.width/4
        cellHeight: gridContentListTable.width/5.5
        model: eventListTableModel
        delegate: contactDelegate
        focus: true
        anchors.margins: 10
//       highlight: Rectangle {
//            color: "#EEEEEE"
//            border.color: "red"
//            radius: 7
//            width: gridContentListTable.cellWidth
//            height: gridContentListTable.cellHeight
//       }
       onCurrentIndexChanged: {
           if(eventListTableModel.count > 0) {
                tableNumber = eventListTableModel.get(0).table_id
               current_table_id = eventListTableModel.get(0).table_id
           }
           MainController.getOrder(tableNumber)
       }
    }
}
