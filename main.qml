import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import com.vng.gbc.smr 1.0

import "controls" as Awesome

ApplicationWindow {
    id: window
    visible: true
    visibility: Qt.platform.os === "android" ? "FullScreen" : "Windowed"
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    property color backGroundColor : "#FFFFFF"
    property color mainAppColor: "#FF9800"
    property color mainTextCOlor: "gray"
    property color popupBackGroundColor: "#b44"
    property color popupTextCOlor: "#ffffff"
    property var dataBase
    property int globalPadding: 10
    property double size_width: Screen.desktopAvailableWidth/480
    property double size_height: Screen.desktopAvailableHeight/720
    property double size_scale: (size_width + size_height) / 2
    property string staff_name: qsTr("")
    property int staff_id: 0
    property int current_table_id: 0
    property string current_invoice_id: qsTr("")
    property string account_name: qsTr("")
    property int select_menu: 0
    property string txtThanhTien: qsTr("")
    property int fntsize: 15

    title: qsTr("SUSHI")

    function isLogin() {
        if(account_name === "")
            return false;
        return true;
    }

    function get_menu() {
        if(select_menu === 0)
            return true;
        return false;
    }

    header: ToolBar {
        Rectangle {
            anchors.fill: parent
            width: window.width
            height: 50

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#ad3906" }
                GradientStop { position: 1.0; color: "#ef8422" }
            }

            GridLayout {
                anchors.fill: parent
                width: window.width
                height: 50
                columns: 3

                RowLayout {
                    id: rleft
                    height: 50
                    width: window.width/3
                    ToolButton {
                        id: btnMenu
                        visible: isLogin()
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: "qrc:/images/drawer.png"
                        }
                        onClicked: optionsMenu.open()

                        Menu {
                            id: optionsMenu
                            x: parent.width - width
                            transformOrigin: Menu.TopLeft

                            MenuItem {
                                text: "Khu vực / Bàn"
                                onTriggered:  {
                                    select_menu = 0
                                }
                            }
                            MenuItem {
                                text: "Thực đơn"
                                onTriggered:  {
                                    select_menu = 1
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    height: 50
                    width: window.width/3
                    anchors.centerIn: parent
                    Text {
                        id: txtTitle
                        text: qsTr("Nhân SUSHI")
                        color: "white"
                        font.family: awesome.family
                        font.pixelSize: 25
                        anchors.centerIn: parent
                    }
                    Button {
                        id: secret
                        property bool allow: false
                        opacity: 0.0
                        text: 'xxxxxx'
                        onClicked: {
                            allow = true
                        }
                    }
                }

                RowLayout {
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    width: window.width/3
                    height: 50
                    Button {
                        opacity: 0.0
                        text: 'xxxxxx'
                        onClicked: {
                            if (secret.allow)
                                stackView.push(settingPage)
                            secret.allow = false
                        }
                    }
                    Label {
                        visible: isLogin()
                        id: headerUserName
                        font.family: awesome.family
                        font.pointSize: 13
                        color: "white"
                        text: "Xin chào " + staff_name
                        renderType: Text.NativeRendering
                        anchors.right: btnUser.left
                    }
                    ToolButton {
                        id: btnUser
                        visible: isLogin()

                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: "qrc:/images/icons/menu.png"
                        }
                        onClicked: menuUser.open()
                        Menu {
                            id: menuUser
                            x: parent.width - width
                            transformOrigin: Menu.TopRight

                            MenuItem {
                                font.family: awesome.family
                                font.pointSize: 13
                                text: awesome.icons.fa_key + "  Đổi mật khẩu"
                                onTriggered: {
                                    popupChangePassword.open()
                                }
                            }
                            MenuItem {
                                font.family: awesome.family
                                font.pointSize: 13
                                text:  awesome.icons.fa_sign_out + "  Thoát"
                                onTriggered: {
                                    staff_name = qsTr("")
                                    account_name = qsTr("")
                                    stackView.pop()
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: popupChangePassword
        x: parent.width/2 - popupChangePassword.width/2
        y: parent.height/2 - popupChangePassword.height/2
        ColumnLayout {
            id: appOptionColumn
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: "Đổi mật khẩu"
                Layout.alignment: Qt.AlignHCenter
                font.bold: true
                font.pixelSize: fntsize + 10
            }

            Row {
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 180
                    text: "Tên: "
                }

                TextField {
                    id: idusrName
                    width: 180
                    text: account_name
                    readOnly:true
                }
            }

            Row {
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 180
                    text: "Mật khẩu cũ: "
                }

                TextField {
                    id: idPassword
                    width: 180
                    text: ""
                    echoMode: TextInput.Password
                    onDisplayTextChanged: loginErrorMsg.text = ""
                }
            }

            Row {
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 180
                    text: "Mật khẩu mới: "
                }

                TextField {
                    id: idNewPassword
                    width: 180
                    text: ""
                    echoMode: TextInput.Password
                    onDisplayTextChanged: loginErrorMsg.text = ""
                }
            }

            Row {
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 180
                    text: "Xác nhận mật khẩu mới: "
                }

                TextField {
                    id: idConfirmNewPassword
                    width: 180
                    text: ""
                    echoMode: TextInput.Password
                    onDisplayTextChanged: loginErrorMsg.text = ""
                }
            }

            Label {
                id: loginErrorMsg
                Layout.alignment: Qt.AlignHCenter
                text: ""
            }

            Row {
                id: rowControl
                spacing: 10
                Layout.alignment: Qt.AlignHCenter

                Button {
                    id: okButton
                    text: "Đồng ý"
                    width: 100
                    onClicked: {

                    }
                }

                Button {
                    id: cancelButton
                    text: "Hủy"
                    width: 100
                    onClicked: {
                        popupChangePassword.close()
                    }
                }
            }
        }
    }


    FontLoader {
        id: fontAwesome
        name: "fontawesome"
        source: "qrc:/fontawesome-webfont.ttf"
    }

    FontLoader {
        id: montserrat;
        source: "qrc:///resource/Montserrat-Regular.ttf"
    }

    FontLoader {
        source: "qrc:///resource/Montserrat-Bold.ttf"
    }

    FontLoader {
        source: "qrc:///resource/Montserrat-Medium.ttf"
    }

    FontLoader {
        source: "qrc:///resource/Montserrat-Light.ttf"
    }

    font.family: montserrat.name

    FontAwesome {
        id: awesome
        resource: "qrc:///resource/fontawesome-webfont.ttf"
    }

    SettingsPage {
        id: settingPage
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Login {
            id: loginPage
        }
        MainPage {
            id: mainPage
        }
    }

    Popup {
        // show bảng thông báo thanh toán Tiền mặt
        id: cashAlert
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: Math.abs(window.height - cashAlert.height) / 3
        closePolicy: Popup.NoAutoClose
        property string cashValue: "0"
        property var _fooditems;

        Column {
            id: cashAlertColumn
            width: implicitWidth
            height: implicitHeight
            spacing: 10

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                topPadding: globalPadding
                bottomPadding: globalPadding

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: fntsize + 4
                    font.weight: Font.DemiBold
                    text: "XÁC NHẬN THANH TOÁN"
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: childrenRect.width + 2
                    height: childrenRect.height + 2
                    Image {
                        x: 1
                        y: 1
                        source: "qrc:/images/icons/images/cash.png"
                    }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Label {
                        text: "Tiền mặt "
                        font.pixelSize: fntsize
                    }
                    Label {
                        id: labelCashvalue
                        font.weight: Font.DemiBold
                        color: "#ff0000"
                        text: cashAlert.cashValue
                        font.pixelSize: fntsize
                    }

                    Label {
                        text: " VNĐ"
                        font.pixelSize: fntsize
                    }
                }

                Row {
                    spacing: globalPadding
                    anchors.horizontalCenter: parent.horizontalCenter
                    Button {
                        width: 140
                        highlighted: true
                        font.pixelSize: fntsize
                        text: "Thanh toán"
                        onClicked: {
                            MainController.pay(cashAlert.cashValue, cashAlert.foodItems, 4)
                            console.log("tableid: " + current_table_id)
                            console.log("staff_id: " + staff_id)
                            console.log("invoice id: " + current_invoice_id)
                            console.log("amount: " + cashAlert.cashValue)
                            MainController.pay(current_table_id, staff_id, current_invoice_id, MainController.moneyNumber(cashAlert.cashValue))

                            var jbill = [];
                            jbill.push(cashAlert._fooditems)
                            console.log("cashAlert._fooditems" + JSON.stringify(cashAlert._fooditems))

                            var fio = { table_id:current_table_id,
                                        table_name: "Ban " + current_table_id,
                                        barcode:"362589764568",
                                        bills:cashAlert._fooditems}

//                            var prnThuNganInfo = {
//                                title:"THUNGAN - NHAN SUSHI",
//                                prefix:"Ban " + current_table_id,
//                                postfix:"",
//                                time:new Date}
                            var prnPaymentInfo = {
                                title:"NHAN SUSHI",
                                prefix:"62 Ngo Duc Ke, P.Ben Nghe, Q.1;;" + "Ban " + current_table_id ,
                                postfix:"Cam on Qui Khach, hen gap lai",
                                time:new Date}

                            MainController.printbill(prnPaymentInfo, fio , 4, "THUNGAN")

                            cashAlert.close()
                        }
                    }
                    Button {
                        width: 140
                        font.pixelSize: fntsize
                        text: "Hủy"
                        onClicked: {
                            MainController.sendCancelBillUI();
                            cashAlert.close()
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: popQuetthe
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: Math.abs(window.height -  popQuetthe.height)/3
        closePolicy: Popup.NoAutoClose

        property string totalBill: "0"

        Column {
            id: qtaboutColumn
            height: implicitHeight
            width: qtrowContent.width
            spacing: 10

            Column {
                id: qtrowContent
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                topPadding: globalPadding
                bottomPadding: globalPadding
                width: implicitWidth + 60

                Label {
                    id: qtlabelTitle
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: fntsize + 4
                    font.weight: Font.DemiBold
                    text: "XÁC NHẬN THANH TOÁN"
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    Label {
                        id: qtlabelQR
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Quét thẻ để thanh toán đơn hàng"
                        font.pixelSize: fntsize
                    }

                    RowLayout {
                        id:qtrowInfos
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            id: qtlabelText1
                            text: " với số tiền "
                            font.pixelSize: fntsize
                        }

                        Label {
                            id: qtlabelThanhtien
                            anchors.left: qtlabelText1.right
                            font.weight: Font.DemiBold
                            color: "#ff0000"
                            text: txtThanhTien
                            font.pixelSize: fntsize
                        }

                        Label {
                            id: qtlabelText2
                            anchors.left: qtlabelThanhtien.right
                            anchors.bottom: qtlabelThanhtien.bottom
                            anchors.bottomMargin: 2
                            color: "#ff0000"
                            font.pointSize: qtlabelThanhtien.font.pointSize*2/3
                            text: " VNĐ"
                        }
                    }
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    AnimatedImage {
                        width: 200
                        height:250
                        source: "qrc:/images/icons/images/docheckin.gif"
                    }
                }

                Row {
                    // code cho DEV only
                    visible: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.fillWidth: true
                    height: 30
                    spacing: 30
                    Label {
                        id: idmathetitle
                        text:"Mã thẻ"
                        //anchors.horizontalCenter: parent.horizontalCenter
                    }
                    TextInput {
                        id: idmathe
                        text: "CFFB7B"
                        width: 100
                        //anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Button {
                        id: idnhanthe
                        text: "Nhận thẻ"
                        //anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                            MainController.giaQuetthe(idmathe.text)
                        }
                    }
                }
            }

            Button {
                width: 200
                font.pixelSize: fntsize
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Hủy"
                onClicked: {
                    MainController.cancelBill("", "", popQuetthe.totalBill);
                    popQuetthe.close()
                }
            }
        }


    }

    Popup {
        // show bảng thanh toán Zalopay
        id: payZaloDialog
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: Math.abs(window.height -  payZaloDialog.height)/3
        closePolicy: Popup.NoAutoClose
        property string totalBill: "0"
        property string foId: "" // foods order ID

        function resetQRCode() {
            qrimage.setImage("")
            labelInvoice.text = ""
            busyIndicatorQR.visible = true
        }

        function doCancelBill() {
            // send signal do cancel bill, and waiting respone ok to close dialog
            MainController.cancelBill(foId, labelInvoice.text, payZaloDialog.totalBill);
        }

        Column {
            id: aboutColumn
            width: implicitHeight
            height: implicitHeight
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Column {
                id: rowContent
                anchors.horizontalCenter: parent.horizontalCenter
                width: implicitWidth
                spacing: 10
                topPadding: globalPadding
                bottomPadding: globalPadding

                Label {
                    id: labelTitle
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: fntsize + 4
                    font.weight: Font.DemiBold
                    text: "XÁC NHẬN THANH TOÁN"
                }

                Row {
                    width: childrenRect.width
                    height: childrenRect.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    Rectangle {
                        width: 10
                        height: qrimage.height
                    }

                    Rectangle {
                        border.width: 1
                        border.color: "#E2E6E7"
                        width: qrimage.width + 4
                        height: qrimage.height + 4
                        Image {
                            id: qrimage
                            visible: false
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            x: 1
                            y: 1
                            function setImage(url) {source=url;busyIndicatorQR.visible=false}
                        }
                    }

                    Rectangle {
                        width: 10
                        height: qrimage.height
                    }
                }

                Label {
                    // show msg payment if fail
                    id: labelError
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: ""
                    font.pixelSize: fntsize
                }

                Column {
                    id:idGroupInfos
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10
                    Label {
                        id: labelQR
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Quét QR code để thanh toán cho hóa đơn"
                        font.pixelSize: fntsize
                    }

                    RowLayout {
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            id: labelInvSign
                            font.weight: Font.DemiBold
                            text: ""
                            font.pixelSize: fntsize
                        }

                        Label {
                            id: labelInvoice
                            anchors.left: labelInvSign.right
                            font.weight: Font.DemiBold
                            text: ""
                            font.pixelSize: fntsize
                        }

                        Label {
                            id: labelText1
                            anchors.left: labelInvoice.right
                            text: " với số tiền "
                            font.pixelSize: fntsize
                        }

                        Label {
                            id: labelGiagoc // giá gốc
                            anchors.left: labelText1.right
                            font.weight: Font.DemiBold
                            visible: iDiscount > 0
                            font.strikeout: true
                            font.pixelSize: fntsize - 2
                            text: txtGiagoc
                        }

                        Label {
                            id: labelThanhtien
                            anchors.left: (iDiscount > 0) ? labelGiagoc.right : labelText1.right
                            font.weight: Font.DemiBold
                            color: "#ff0000"
                            text: txtThanhTien
                            font.pixelSize: fntsize
                        }

                        Label {
                            id: labelText2
                            anchors.left: labelThanhtien.right
                            anchors.bottom: labelThanhtien.bottom
                            anchors.bottomMargin: 2
                            color: "#ff0000"
                            font.pixelSize: labelThanhtien.font.pixelSize*2/3
                            text: " VNĐ"
                        }
                    }
                }

                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: idGroupTimer
                    BusyIndicator {
                        id: busyIndicatorQR
                        width: 60
                        height: 60
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Row {
                width: rowContent.width
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    height: 60
                    width: parent.width/2
                    highlighted: true
                    font.pixelSize: fntsize + 4
                    text: "In Phiếu"
                    onClicked: {


                    }
                }

                Button {
                    height: 60
                    width: parent.width/2
                    highlighted: true
                    font.pixelSize: fntsize + 4
                    text: "Hủy"
                    onClicked: {
                        payZaloDialog.doCancelBill()
                        payZaloDialog.close()
                    }
                }
            }
        }
    }

    Popup {
        id: errAlert
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: (window.height -  height)/2
        closePolicy: Popup.NoAutoClose

        contentItem: Column {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                text: "Mạng Lỗi"
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Column {
                Layout.fillWidth: true
                Label {
                    id: errAlertErr
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Mã lỗi: "
                }

                Label {
                    id: errAlertInfo
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Thông tin lỗi: "
                }
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 100
                highlighted: true
                font.pointSize: 16
                text: "Đóng"
                onClicked: {
                    errAlert.close()
                }
            }
        }
    }
}
