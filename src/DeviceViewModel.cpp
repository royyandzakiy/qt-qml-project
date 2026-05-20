#include "DeviceViewModel.h"
#include <QSerialPortInfo>

DeviceViewModel::DeviceViewModel(QObject* parent)
    : QObject(parent)
{
    refreshPorts();
}

void DeviceViewModel::onRefreshPorts() {
    refreshPorts();
}

void DeviceViewModel::refreshPorts() {
    QStringList ports;
    for (const auto& info : QSerialPortInfo::availablePorts()) {
        QString entry = info.portName();
        if (!info.description().isEmpty())
            entry += "  —  " + info.description();
        ports.append(entry);
    }
    if (m_availablePorts == ports) return;
    m_availablePorts = ports;
    emit availablePortsChanged();
}
