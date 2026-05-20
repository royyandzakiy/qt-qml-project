import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    ColumnLayout {
        anchors.centerIn: parent
        width: 320
        spacing: 12

        Label {
            text: "Cashier App"
            font.pixelSize: 26
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Sign In"
            font.pixelSize: 15
            color: "#666"
            Layout.alignment: Qt.AlignHCenter
        }

        Item { height: 8 }

        Label { text: "Username" }
        TextField {
            id: usernameField
            Layout.fillWidth: true
            placeholderText: "Enter username"
            Keys.onReturnPressed: passwordField.forceActiveFocus()
        }

        Label { text: "Password" }
        TextField {
            id: passwordField
            Layout.fillWidth: true
            echoMode: TextInput.Password
            placeholderText: "Enter password"
            Keys.onReturnPressed: authVm.onLogin(usernameField.text, passwordField.text)
        }

        Label {
            text: authVm.errorMessage
            color: "red"
            visible: authVm.errorMessage !== ""
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Button {
            text: "Login"
            Layout.fillWidth: true
            onClicked: authVm.onLogin(usernameField.text, passwordField.text)
        }
    }
}
