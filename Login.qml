import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import com.vng.gbc.smr 1.0
import "backend.js" as Backend

Page {
    id: loginPage
    signal registerClicked()
    property string logindata: qsTr("")
    Connections {
        target: MainController
        onLoginConnection: {
            logindata = data;
        }
    }

    background: Rectangle {
        Image {
            source: "qrc:/images/login-screen.jpg"
            height: parent.height
            width: parent.width
        }
    }

    Connections {
        target: MainController

    }

    ColumnLayout {
        id: iconRect
        width: parent.width
        height: parent.height / 3
        Text {
            id: icontext
            text: qsTr("\uf169")
            anchors.centerIn: parent
            font.pointSize: 112
            font.family: "fontawesome"
            color: mainAppColor
        }
    }
    ColumnLayout {
            width: parent.width/4
            anchors.top: iconRect.bottom
            spacing: 15
            anchors.horizontalCenter: parent.horizontalCenter
            TextField {
                id: loginUsername
                placeholderText: qsTr("Tài khoản")
                Layout.preferredWidth: parent.width
                Layout.alignment: Qt.AlignHCenter
                color: "gray"
                font.pointSize: 14
                font.family: "fontawesome"
                leftPadding: 50
                text: ""
                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 50
                    radius: implicitHeight / 2
                    color: "white"

                    Text {
                        text: "\uf007"
                        font.pointSize: 20
                        font.family: "fontawesome"
                        color: "gray"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        bottomPadding: 10
                        leftPadding: 20
                    }
                }
            }

            TextField {
                id: loginPassword
                placeholderText: qsTr("Mật khẩu")
                Layout.preferredWidth: parent.width
                Layout.alignment: Qt.AlignHCenter
                color: "gray"
                font.pointSize: 14
                font.family: "fontawesome"
                leftPadding: 50
                echoMode: TextField.Password
                text: ""
                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 50
                    radius: implicitHeight / 2
                    color: "white"
                    Text {
                        text: "\uf023"
                        font.pointSize: 20
                        font.family: "fontawesome"
                        color: "gray"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        bottomPadding: 10
                        leftPadding: 20
                    }
                }
            }

            CButton{
                height: 50
                Layout.preferredWidth: Screen.width/4
                Layout.alignment: Qt.AlignHCenter
                name: "Đăng nhập"
                baseColor: "#2196F3"
                borderColor: "#2196F3"
                onClicked: {
                    window.width = Screen.width
                    window.height = Screen.height
                    window.x = 0
                    var u = loginUsername.text
                    var p = loginPassword.text
                    MainController.login(u,p)
                    var json_datalogin = JSON.parse(logindata);
                    var err = json_datalogin.err;
                    staff_name = json_datalogin.dt.staff.staff_name;
                    account_name = json_datalogin.dt.staff.account;
                    staff_id = json_datalogin.dt.staff.staff_id;
                    if(err === 0) {
                        stackView.pop(mainPage)
                        stackView.push(mainPage)
                    }
                }
            }
        }
}
