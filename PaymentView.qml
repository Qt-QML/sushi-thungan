import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import com.vng.gbc.smr 1.0

Rectangle {
    height: childrenRect.height
    color: "transparent"
    property var citlv: null
    property int txtfntsize: 15

    Column {
        width: parent.width
        spacing: 20

        Image {
            id: imageSeparate
            height: 20
            fillMode: Image.Stretch
            cache: true
            source: "qrc:/images/icons/rangcua.png"
            width: parent.width
        }

        Column {
            width: parent.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: window.globalPadding

            RowLayout {
                Layout.fillWidth: true
                width: parent.width

                Text {
                    id: textThanhtienLabel
                    height: 85
                    anchors.left: parent.left
                    text: qsTr("Tổng tiền:")
                    font.pointSize: txtfntsize + 2
                }

                Text {
                    id: textThanhtienVnd
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 3
                    height: 85
                    text: qsTr("VNĐ")
                    font.pointSize: txtfntsize
                    font.weight: Font.DemiBold
                }

                Text {
                    id: textThanhtienValue
                    anchors.right: textThanhtienVnd.left
                    anchors.margins: 10
                    height: 85
                    text: MainController.moneyString(total_amount)
                    font.pointSize: txtfntsize + 2
                    font.weight: Font.DemiBold
                }
            }

            RowLayout {
                id: idPaymethod
                Layout.fillWidth: true
                width: parent.width
                RadioButton {
                    id: payZalo
                    height: 85
                    checked: true
                    text: qsTr("Zalopay")
                    onClicked: {
                        payment_type = 1;
                    }
                }

                RadioButton {
                    id: payCard
                    text: qsTr("Thẻ")
                    onClicked: {
                        payment_type = 2;
                    }
                }

                RadioButton {
                    id: payCash
                    text: qsTr("Tiền mặt")
                    onClicked: {
                        payment_type = 4;
                    }
                }
            }

            Button {
                id: btnThanhToan
                anchors.left: parent.left
                anchors.right: parent.right
                height: 80
                text: qsTr("Thanh toán")
                highlighted: total_amount > 0 ? true : false
                font.pointSize: txtfntsize + 8
                onClicked: {
                    if(btnThanhToan.highlighted) {
                        // set bill detail to mainController
                        var foodItems = orderModel.billDetail()
                        window.txtThanhTien = textThanhtienValue.text

                        if (payment_type === 4) {
                            // thanh toán bằng tiền mặt: gửi request hóa đơn lên server, OK in hóa đơn thu tiền
                            cashAlert.cashValue = textThanhtienValue.text
                            cashAlert._fooditems = foodItems;
                            cashAlert.open()
                        }
                        else if (payment_type === 2) {
                            // thanh toán thẻ, chờ quẹt thẻ, gửi request hóa đơn lên server, OK in hóa đơn giao khách
                            popQuetthe.totalBill = textThanhtienValue.text
                            popQuetthe.open()
                        }
                        else if (payment_type === 1) {
                            // thanh toán qua Zalopay, gửi request lên server, nhận về qr-code show ra và chờ Zalopay gửi tín hiệu thành công để in hóa đơn giao khách
                            var foId = MainController.pay(textThanhtienValue.text, foodItems, 1)
                            payZaloDialog.resetQRCode();
                            payZaloDialog.totalBill = textThanhtienValue.text
                            payZaloDialog.foId = foId;
                            payZaloDialog.open()
                        }
                        else {

                        }
                    }
                }
            }
            Connections {
                target: MainController
                onPaySucc: {
                    // update QR code
                    console.log("qrcode: " + qrcode)
                    qrimage.visible = true
                    qrimage.setImage("image://colors/" + qrcode)

                    window.txtThanhTien = discpay

                    // enable controls
                    idGroupInfos.visible = true;
                    idGroupTimer.visible = true;
                    labelError.visible = false;
                    labelInvoice.text = "#" + invcecode;
                }

                onPayError: {
                    // disable controls
                    idGroupInfos.visible = false;
                    idGroupTimer.visible = false;
                    labelError.visible = true;
                    labelError.text = msg;
                }

                onErrorMessage: {
                    errAlertErr.text = "Mã lỗi: " + err;
                    errAlertInfo.text = "Thông tin lỗi: " + msg;
                    errAlert.open();
                }
            }
        }
    }
}
