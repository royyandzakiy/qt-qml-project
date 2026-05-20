#pragma once
#include <QObject>
#include <QStringList>
#include <QtQmlIntegration/qqmlintegration.h>
class QQmlEngine;
class QJSEngine;

class DeviceViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(DeviceVM)
    QML_SINGLETON

public:
    explicit DeviceViewModel(QObject* parent = nullptr);

    static DeviceViewModel* create(QQmlEngine*, QJSEngine*) {
        return new DeviceViewModel;
    }

    QStringList availablePorts() const { return m_availablePorts; }

    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)

signals:
    void availablePortsChanged();

public slots:
    void onRefreshPorts();

private:
    QStringList m_availablePorts;

    void refreshPorts();
};
