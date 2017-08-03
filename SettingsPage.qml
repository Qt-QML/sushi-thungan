import QtQuick 2.0
import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0

import com.vng.gbc.smr 1.0

Page {
    Pane {
        id: settingsPane
        property bool changed: false

        anchors.fill: parent
        ColumnLayout {
            id: settingsColumn
            spacing: 20
            Layout.fillWidth: true
            anchors.horizontalCenter: parent.horizontalCenter
            // Row setting server url
            RowLayout {
                spacing: 10
                Label {
                    text: "Server:"
                }

                TextArea {

                    id: txtServerUrl
                    Layout.fillWidth: true
                    text: AppSetting.serverUrl()
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setServerUrl(txtServerUrl.text);
                        AppSetting.save();
                        console.log("already save");
                    }
                }
            }

            RowLayout {
                spacing: 10
                Label {
                    text: "Desc:"
                }

                TextArea {
                    id: txtDescription
                    Layout.fillWidth: true
                    text: AppSetting.description()
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setDescription(txtDescription.text);
                        AppSetting.save();
                    }
                }
            }

            RowLayout {
                spacing: 10
                Label {
                    text: "Merchant:"
                }

                TextArea {
                    id: txtMerchantCd
                    Layout.fillWidth: true
                    text: AppSetting.merchantCode()
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setMerchantCode(txtMerchantCd.text);
                        AppSetting.save();
                    }
                }
            }

            RowLayout {
                spacing: 10
                Label {
                    text: "Machine:"
                }

                TextArea {
                    id: txtMachineName
                    Layout.fillWidth: true
                    text: AppSetting.machineName()
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setMachineName(txtMachineName.text);
                        AppSetting.save();
                    }
                }
            }

            RowLayout {
                spacing: 10
                Label {
                    text: "WS Url:"
                }

                TextArea {
                    id: txtWsUrl
                    Layout.fillWidth: true
                    text: AppSetting.wsUrl()
                }

                Button {
                    text: "Save"
                    onClicked: {
                        AppSetting.setWsUrl(txtWsUrl.text);
                        AppSetting.save();
                    }
                }
            }

            // Row save settings button
            RowLayout {
                spacing: 10

                Button {
                    text: "Back"
                    onClicked: {
                        if (stackView.depth > 1) {
                            stackView.pop()
                        }
                    }
                }
            }
        }
    }
}
