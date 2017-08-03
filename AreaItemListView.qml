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
    id: rectContentGroup
    Component {
       id: contentGroupDelegate
       Item {
           id: itemGroup
           width: gridContentGroup.cellWidth;
           height: gridContentGroup.cellHeight
           Column {
               anchors.fill: parent
               spacing: 0
               HalfRadiusRectangle {
                   id: rectComponentItemInGroup
                   anchors.centerIn: parent
                   rcColor: itemGroup.GridView.isCurrentItem? "#FF5722":"#E2E6E7"
                   width: gridContentGroup.cellWidth-10;
                   height: gridContentGroup.cellHeight-10
                   ColumnLayout {
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        Text {
                            text: area_name
                            font.family: awesome.family
                            font.pixelSize: 20
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                   }
                   MouseArea {
                       anchors.fill: parent
                        onClicked: {
                            var local_tableinareas = []
                            gridContentGroup.currentIndex = index
                            areaName = area_name
                            for(var i in tables) {
                                if(areaName === tables[i].area_name) {
                                    local_tableinareas.push(tables[i])
                                }
                            }
                            eventListTableModel.clear()
                            eventListTableModel.append(local_tableinareas)
                            if(eventListTableModel.count > 0) {
                                MainController.getOrder(eventListTableModel.get(0).table_id)
                            }
                        }
                   }
               }
           }
       }
    }

    GridView {
       id: gridContentGroup
       anchors.fill: parent
       anchors.margins: 0
       clip: true
       cellWidth: rectContentGroup.width;
       cellHeight: rectContentGroup.width
       model: eventModelContentGroup
       delegate: contentGroupDelegate
       highlight: Rectangle {
            color: "#EEEEEE"
            border.color: "red"
            radius: 7
            width: gridContentGroup.cellWidth-5;
            height: gridContentGroup.cellHeight-5

       }
       focus: true
       onCurrentIndexChanged: {
           if(eventModelContentGroup.count > 0) {
                areaName = eventModelContentGroup.get(0).area_name
           }
           var tableInAreas = [];
           for(var i in tables) {
               if(areaName === tables[i].area_name) {
                   tableInAreas.push(tables[i])
               }
           }
           eventListTableModel.clear()
           eventListTableModel.append(tableInAreas)
       }
    }
}

