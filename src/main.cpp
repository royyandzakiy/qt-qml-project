#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "AuthViewModel.h"
#include "CashierViewModel.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Cashier App");
    app.setOrganizationName("LMesh");

    QQmlApplicationEngine engine;

    auto* authVm    = new AuthViewModel(&engine);
    auto* cashierVm = new CashierViewModel(&engine);
    engine.rootContext()->setContextProperty("authVm",    authVm);
    engine.rootContext()->setContextProperty("cashierVm", cashierVm);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
