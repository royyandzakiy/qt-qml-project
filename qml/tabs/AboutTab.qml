import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        GroupBox {
            title: "About"
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 4
                Label { text: "Cashier App  v1.0";               font.bold: true }
                Label { text: "Simple point-of-sale application"; color: "#555" }
                Label { text: "Organization: Compacompa";              color: "#555" }
            }
        }

        GroupBox {
            title: "Devices"
            Layout.fillWidth: true

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 6

                RowLayout {
                    spacing: 8
                    Label { text: "COM Port:" }
                    ComboBox {
                        id: portCombo
                        model: deviceVm.availablePorts
                        Layout.fillWidth: true
                        displayText: count === 0 ? "No devices connected" : currentText
                    }
                    Button {
                        text: "Refresh"
                        implicitWidth: 80
                        onClicked: deviceVm.onRefreshPorts()
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
