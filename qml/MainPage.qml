import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "tabs"

Item {
    TabBar {
        id: tabBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        TabButton {
            text: "Cashier"
            background: Rectangle { color: parent.checked ? "#f0f0f0" : "#dcdcdc" }
            contentItem: Text {
                text: parent.text
                color: "#222222"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        TabButton {
            text: "About"
            background: Rectangle { color: parent.checked ? "#f0f0f0" : "#dcdcdc" }
            contentItem: Text {
                text: parent.text
                color: "#222222"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    StackLayout {
        anchors {
            top:    tabBar.bottom
            left:   parent.left
            right:  parent.right
            bottom: parent.bottom
        }
        currentIndex: tabBar.currentIndex

        CashierTab {}
        AboutTab   {}
    }
}
