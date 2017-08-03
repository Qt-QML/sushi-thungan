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
    property int iconsize: Screen.devicePixelRatio < 2 ? 200 : 120 / Screen.devicePixelRatio
    property int fooditemfntsize: 13
    property int m_colCount: Math.min(width < iconsize ? 1 : width / iconsize, 4)
    property int m_colSpace: width / 25
    property int m_cellw: width / m_colCount
    property int m_cellh: m_cellw+30

    property bool itmfilter: false
    property string presrchtxt: ""
    color: "white"

    GridView {
        id: gridItemView
        anchors.fill: parent
        cellWidth: m_cellw
        cellHeight: m_cellh
        focus: true
        model: eventListFood

        delegate: Item {
            id: item
            width: m_cellw
            height: m_cellh

            property int cellPadding: 3
            property int cellFontSize: fooditemfntsize

            HalfRadiusRectangle {
                id: halfRect
                anchors.fill: parent
                anchors.leftMargin: cellPadding
                anchors.rightMargin: cellPadding
                anchors.bottomMargin: cellPadding*2
                property bool viewitem : true

                bottomRadius: 10
                radiusWidth: 1
                rcColor: item.GridView.isCurrentItem? "#FF5722":"#E2E6E7"

                // search-filter
                opacity: itmviewable

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10
                    Item {
                        id: itemImage
                        anchors.top: parent.top
                        width:parent.width
                        height:width*255/310
                        Image {
                            id: foodIcon
                            fillMode: Image.Stretch
                            anchors.fill: parent
                            anchors.margins: 2
                            asynchronous: true
                            cache: img_cache
                            source: img_path
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                        anchors.top: itemImage.bottom
                        anchors.bottom: parent.bottom

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 5 / Screen.devicePixelRatio
                            Label {
                                id:foodName
                                text: item_name
                                font.pixelSize: cellFontSize
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter

                                //--- text multi lines ...
                            }

                            RowLayout {
                                anchors.horizontalCenter: parent.horizontalCenter

                                Text {
                                    text: oriprice
                                    //text: promtype
                                    color: "#4a9fe7"
                                    font.pixelSize: cellFontSize + 2
                                    font.strikeout: true
                                }

                                Text {
                                    text: qsTr(Number(price).toLocaleString(Qt.locale("de_DE"))).replace(",00","")
                                    color: "#FF0000"
                                    font.pixelSize: cellFontSize + 2
                                }

                                Text {
                                    text: "VNĐ"
                                    color: "#FF0000"
                                    font.pixelSize: cellFontSize
                                    anchors.bottom: parent.bottom
                                    anchors.margins: 4
                                }
                            }
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        gridItemView.currentIndex = index
                        orderModel.addItem(item_name,
                                              1,
                                              price,
                                              price
                                          )
                    }
                }
            }
        }
    }
}
