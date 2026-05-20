#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Cashier App");
    app.setOrganizationName("Compacompa");

    QQmlApplicationEngine engine;
    engine.loadFromModule("CashierApp", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
