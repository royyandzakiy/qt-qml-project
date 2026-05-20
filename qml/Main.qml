import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 700
    minimumWidth: 700
    minimumHeight: 500
    title: "Cashier App"

    StackLayout {
        anchors.fill: parent
        currentIndex: authVm.isLoggedIn ? 1 : 0

        LoginPage {}
        MainPage  {}
    }
}
