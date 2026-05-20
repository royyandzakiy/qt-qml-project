#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "AuthViewModel.h"
#include "CashierViewModel.h"
#include "DeviceViewModel.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Cashier App");
    app.setOrganizationName("Compacompa");

    QQmlApplicationEngine engine;

    auto* authVm    = new AuthViewModel(&engine);
    auto* cashierVm = new CashierViewModel(&engine);
    auto* deviceVm  = new DeviceViewModel(&engine);
    engine.rootContext()->setContextProperty("authVm",    authVm);
    engine.rootContext()->setContextProperty("cashierVm", cashierVm);
    engine.rootContext()->setContextProperty("deviceVm",  deviceVm);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
