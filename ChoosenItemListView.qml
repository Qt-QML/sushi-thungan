import QtQuick 2.7
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Private 1.0
import QtQuick.Window 2.0
import com.vng.gbc.smr 1.0
Item {
    TableView {
        id: tableViewChoosen
        anchors.fill: parent
        property int colRatio: width/20
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        TableViewColumn {
            resizable: false
            movable: false
            role: "item_name"
            title: "Tên món"
            width: tableViewChoosen.colRatio*7
        }
        TableViewColumn {
            resizable: false
            movable: false
            role: "quantity"
            title: "SL"
            width: tableViewChoosen.colRatio*2
        }
        TableViewColumn {
            resizable: false
            movable: false
            role: "price"
            title: "Giá"
            width: tableViewChoosen.colRatio*4
        }
        TableViewColumn {
            resizable: false
            movable: false
            role: "amount"
            title: "Thành tiền"
            width: tableViewChoosen.colRatio*5
        }
        TableViewColumn {
            resizable: false
            movable: false
            role: "cancel"
            title: qsTr("")
            width: tableViewChoosen.colRatio*2
            delegate: imageDelegate
        }

        model: orderModel
        style: TableViewStyle {
            frame: Rectangle {
                border {
                    color: "transparent" // color of the border
                }
                color: "transparent"
            }
            headerDelegate: Rectangle {
                implicitHeight: 30 * size_height
                color: "#e2e6e7"
                Text {
                    id: textItem
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: (styleData.column === 0 ? Text.AlignLeft :
                                         ((styleData.column === 4 || styleData.column === 1) ? Text.AlignHCenter:Text.AlignRight))
                    text: styleData.value
                    font.bold: true
                    font.pixelSize: 8 * size_scale
                    elide: Text.ElideRight
                    color: "#404040"
                    renderType: Settings.isMobile ? Text.QtRendering : Text.NativeRendering
                }

                Rectangle {
                    width: parent.width
                    height: 1 * size_height
                    y: parent.height
                    color: "white"
                }

                Rectangle {
                    width: parent.width
                    height: 1 * size_height
                    y: parent.height
                    color: "white"
                }

                Rectangle {
                    width: parent.width
                    height: 1 * size_height
                    y:0
                    color: "white"
                }
            }

            itemDelegate: Item {
                height: 30 * size_height
                property int implicitWidth: label.implicitWidth + 20 * size_width

                Text {
                    id: label
                    objectName: "label"
                    width: parent.width - x - (horizontalAlignment === Text.AlignRight ? 8 : 1)
                    x: (styleData.hasOwnProperty("depth") && styleData.column === 0) ? 0 :
                                                                                       horizontalAlignment === Text.AlignRight ? 1 : 8
                    verticalAlignment:  Text.AlignVCenter
                    horizontalAlignment: (styleData.column === 0 ? Text.AlignLeft :
                                         ((styleData.column === 4 || styleData.column === 1) ? Text.AlignHCenter:Text.AlignRight))

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: 1
                    elide: styleData.elideMode
                    text: styleData.value !== undefined ? styleData.value : ""
                    font.pixelSize: 8 * size_scale//billitemfntsize
                    color: styleData.textColor
                    renderType: Settings.isMobile ? Text.QtRendering : Text.NativeRendering
                }

                MouseArea {
                    id:mouseArea
                    anchors.fill: parent
                    onClicked: {
                        orderModel.removeOne(label.text, 1);
                    }
                }
            }

            rowDelegate: Rectangle {
                id: abc
                height: Screen.devicePixelRatio <= 2? 40: 24
                property color selectedColor: "transparent"//control.activeFocus ? "#07c" : "#999"
                color: styleData.selected ? selectedColor : !styleData.alternate ? "#FFFFFF" : "#F5F5F5"
            }
        }
        Component {
            id: imageDelegate
            Item {
                anchors.fill: parent
                Image {
                    fillMode: Image.Pad
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    source:"qrc:/images/icons/cancel.png"
                }

                MouseArea {
                    id:mouseArea
                    anchors.fill: parent
                    onClicked: {
                        // click at ROW to delete
                        console.log("row: " + styleData.row)
                        orderModel.removeItem(styleData.row)
                    }
                }
            }
        }
    }
}


