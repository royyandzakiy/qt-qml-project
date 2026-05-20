import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 8

        // ── New Sale ─────────────────────────────────────────────────────────
        GroupBox {
            title: "New Sale"
            Layout.fillWidth: true

            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10

                Label { text: "Buyer Name:" }
                TextField {
                    implicitWidth: 180
                    placeholderText: "Enter buyer name"
                    text: cashierVm.buyerName
                    onTextEdited: cashierVm.setBuyerName(text)
                }

                Label { text: "Item:" }
                ComboBox {
                    model: cashierVm.availableItems
                    implicitWidth: 140
                    onCurrentIndexChanged: cashierVm.setSelectedItemIndex(currentIndex)
                }

                Label { text: "Price:" }
                Label {
                    text: "Rp " + cashierVm.currentPrice
                    font.bold: true
                    color: "#0078d4"
                    implicitWidth: 90
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: "Add"
                    implicitWidth: 80
                    onClicked: cashierVm.onAddRecord()
                }
            }
        }

        // ── Records ──────────────────────────────────────────────────────────
        GroupBox {
            title: "Records  (" + cashierVm.totalRecords + ")"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // Header row
                Rectangle {
                    Layout.fillWidth: true
                    height: 26
                    color: "#e8e8e8"
                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 6
                        anchors.rightMargin: 6
                        spacing: 0
                        Label { text: "#";          Layout.preferredWidth: 36;  font.bold: true; verticalAlignment: Text.AlignVCenter }
                        Label { text: "Timestamp";  Layout.preferredWidth: 155; font.bold: true; verticalAlignment: Text.AlignVCenter }
                        Label { text: "Buyer";      Layout.preferredWidth: 160; font.bold: true; verticalAlignment: Text.AlignVCenter }
                        Label { text: "Item";       Layout.preferredWidth: 115; font.bold: true; verticalAlignment: Text.AlignVCenter }
                        Label { text: "Price";      Layout.preferredWidth: 90;  font.bold: true; verticalAlignment: Text.AlignVCenter }
                        Item  { Layout.fillWidth: true }
                        Item  { Layout.preferredWidth: 74 }
                    }
                }

                ListView {
                    id: recordsView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: cashierVm.records
                    clip: true

                    delegate: Rectangle {
                        width: recordsView.width
                        height: 26
                        color: index % 2 === 0 ? "#ffffff" : "#f7f7f7"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 6
                            anchors.rightMargin: 6
                            spacing: 0

                            Label { text: modelData.id;        Layout.preferredWidth: 36;  verticalAlignment: Text.AlignVCenter; height: parent.height }
                            Label { text: modelData.timestamp; Layout.preferredWidth: 155; verticalAlignment: Text.AlignVCenter; height: parent.height; font.pixelSize: 11 }
                            Label { text: modelData.buyerName; Layout.preferredWidth: 160; verticalAlignment: Text.AlignVCenter; height: parent.height; elide: Text.ElideRight }
                            Label { text: modelData.item;      Layout.preferredWidth: 115; verticalAlignment: Text.AlignVCenter; height: parent.height }
                            Label { text: "Rp " + modelData.price; Layout.preferredWidth: 90; verticalAlignment: Text.AlignVCenter; height: parent.height }
                            Item  { Layout.fillWidth: true }
                            Button {
                                text: "Remove"
                                Layout.preferredWidth: 74
                                implicitHeight: 22
                                onClicked: cashierVm.onRemoveRecord(index)
                            }
                        }
                    }
                }
            }
        }

        // ── Log + Export ─────────────────────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            GroupBox {
                title: "Log"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 4

                    RowLayout {
                        spacing: 6
                        Label { text: "Session:" }
                        Label {
                            id: sessionLabel
                            text: "00:00:00"
                            font.bold: true
                            font.family: "Courier New"

                            property int elapsed: 0

                            Timer {
                                interval: 1000
                                running: true
                                repeat: true
                                onTriggered: {
                                    sessionLabel.elapsed++
                                    var h = Math.floor(sessionLabel.elapsed / 3600)
                                    var m = Math.floor((sessionLabel.elapsed % 3600) / 60)
                                    var s = sessionLabel.elapsed % 60
                                    sessionLabel.text =
                                        (h < 10 ? "0" : "") + h + ":" +
                                        (m < 10 ? "0" : "") + m + ":" +
                                        (s < 10 ? "0" : "") + s
                                }
                            }
                        }
                    }

                    ListView {
                        Layout.fillWidth: true
                        height: 100
                        model: cashierVm.log
                        clip: true
                        delegate: Text {
                            width: ListView.view.width
                            text: modelData
                            font.pixelSize: 11
                            font.family: "Courier New"
                            padding: 2
                        }
                    }
                }
            }

            GroupBox {
                title: "Export"
                Layout.preferredWidth: 280

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 6

                    Label { text: "Save to file:" }

                    RowLayout {
                        Layout.fillWidth: true
                        TextField {
                            readOnly: true
                            text: cashierVm.exportPath
                            Layout.fillWidth: true
                            placeholderText: "No file selected"
                        }
                        Button {
                            text: "..."
                            implicitWidth: 32
                            onClicked: cashierVm.onSelectExportPath()
                        }
                    }

                    Button {
                        text: "Export to CSV"
                        Layout.fillWidth: true
                        onClicked: cashierVm.onExport()
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
