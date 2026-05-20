#pragma once
#include <QObject>
#include <QStringList>

class DeviceViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)

public:
    explicit DeviceViewModel(QObject* parent = nullptr);

    QStringList availablePorts() const { return m_availablePorts; }

signals:
    void availablePortsChanged();

public slots:
    void onRefreshPorts();

private:
    QStringList m_availablePorts;

    void refreshPorts();
};
