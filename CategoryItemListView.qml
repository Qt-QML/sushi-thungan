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
    id: rectContentCategory
    Component {
       id: contentCategoryDelegate
       Item {
           id: itemCategory
           width: gridContentCategory.cellWidth;
           height: gridContentCategory.cellHeight
           Column {
               anchors.fill: parent
               spacing: 0
               HalfRadiusRectangle {
                   id: rectComponentItemInGroup
                   anchors.centerIn: parent
                   rcColor: itemCategory.GridView.isCurrentItem? "#FF5722":"#E2E6E7"
                   width: gridContentCategory.cellWidth-10;
                   height: gridContentCategory.cellHeight-10
                   ColumnLayout {
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        Text {
                            text: category_name
                            font.family: awesome.family
                            font.pixelSize: 15
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                   }
                   MouseArea {
                       anchors.fill: parent
                        onClicked: {
                            var local_categorys = []
                            gridContentCategory.currentIndex = index
                            categoryName = category_name
                            if(categoryName !== "Tất cả") {
                                for(var i in foods) {
                                    if(categoryName === foods[i].category_name) {
                                        local_categorys.push(foods[i])
                                    }
                                }
                                eventListFood.clear()
                                eventListFood.append(local_categorys)
                            } else {
                                eventListFood.clear()
                                eventListFood.append(foods)
                            }
                        }
                   }
               }
           }
       }
    }

    GridView {
       id: gridContentCategory
       anchors.fill: parent
       anchors.margins: 0
       clip: true
       cellWidth: rectContentCategory.width
       cellHeight: rectContentCategory.width/3.5
       model: eventListCategory
       delegate: contentCategoryDelegate
       highlight: Rectangle {
            color: "#EEEEEE"
            border.color: "red"
            radius: 7
            width: gridContentCategory.cellWidth-5;
            height: gridContentCategory.cellHeight-5

       }
       focus: true
       onCurrentIndexChanged: {
           if(eventListCategory.count > 0) {
                categoryName = eventListCategory.get(0).category_name
           }
       }
    }
}

