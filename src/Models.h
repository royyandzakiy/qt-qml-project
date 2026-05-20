#pragma once
#include <QString>
#include <QDateTime>

namespace models {

struct SaleRecord {
    int       id;
    QString   buyerName;
    QString   item;
    int       price;
    QDateTime timestamp;
};

} // namespace models
