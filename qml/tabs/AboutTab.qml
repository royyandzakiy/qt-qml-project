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
                Label { text: "Organization: LMesh";              color: "#555" }
            }
        }

        GroupBox {
            title: "Devices"
            Layout.fillWidth: true

            RowLayout {
                spacing: 8
                Label { text: "USB Devices:" }
                ComboBox {
                    model: []
                    implicitWidth: 220
                    displayText: count === 0 ? "No devices" : currentText
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
