#include "CashierViewModel.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

static const QString DB_CONNECTION = "cashier_db";

CashierViewModel::CashierViewModel(QObject* parent)
    : QObject(parent)
    , m_availableItems({"Cheetos", "Cornetto", "Chocolatos"})
{
    initDb();
    loadRecords();
}

CashierViewModel::~CashierViewModel() {
    QSqlDatabase::database(DB_CONNECTION).close();
    QSqlDatabase::removeDatabase(DB_CONNECTION);
}

void CashierViewModel::initDb() {
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    auto db = QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION);
    db.setDatabaseName(dataDir + "/cashier.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database",
            "Cannot open database:\n" + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.exec(
        "CREATE TABLE IF NOT EXISTS sales ("
        "  id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  buyerName TEXT    NOT NULL,"
        "  item      TEXT    NOT NULL,"
        "  price     INTEGER NOT NULL,"
        "  timestamp TEXT    NOT NULL"
        ")"
    );
}

void CashierViewModel::loadRecords() {
    auto db = QSqlDatabase::database(DB_CONNECTION);
    QSqlQuery q("SELECT id, buyerName, item, price, timestamp FROM sales ORDER BY id ASC", db);

    m_records.clear();
    while (q.next()) {
        models::SaleRecord rec;
        rec.id        = q.value(0).toInt();
        rec.buyerName = q.value(1).toString();
        rec.item      = q.value(2).toString();
        rec.price     = q.value(3).toInt();
        rec.timestamp = QDateTime::fromString(q.value(4).toString(), "yyyy-MM-dd HH:mm:ss");
        m_records.append(rec);
    }

    emit recordsChanged();
}

int CashierViewModel::currentPrice() const {
    return priceForIndex(m_selectedItemIndex);
}

QVariantList CashierViewModel::recordsVariant() const {
    QVariantList result;
    for (const auto& r : m_records) {
        QVariantMap map;
        map["id"]        = r.id;
        map["buyerName"] = r.buyerName;
        map["item"]      = r.item;
        map["price"]     = r.price;
        map["timestamp"] = r.timestamp.toString("yyyy-MM-dd HH:mm:ss");
        result.append(map);
    }
    return result;
}

void CashierViewModel::setBuyerName(const QString& v) {
    if (m_buyerName == v) return;
    m_buyerName = v;
    emit buyerNameChanged();
}

void CashierViewModel::setSelectedItemIndex(int v) {
    if (m_selectedItemIndex == v) return;
    m_selectedItemIndex = v;
    emit selectedItemIndexChanged();
    emit currentPriceChanged();
}

void CashierViewModel::onAddRecord() {
    if (m_buyerName.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Cashier", "Please enter a buyer name.");
        return;
    }

    const QString item  = m_availableItems.value(m_selectedItemIndex);
    const int     price = priceForIndex(m_selectedItemIndex);
    const auto    now   = QDateTime::currentDateTime();

    auto db = QSqlDatabase::database(DB_CONNECTION);
    QSqlQuery q(db);
    q.prepare("INSERT INTO sales (buyerName, item, price, timestamp) VALUES (?, ?, ?, ?)");
    q.addBindValue(m_buyerName.trimmed());
    q.addBindValue(item);
    q.addBindValue(price);
    q.addBindValue(now.toString("yyyy-MM-dd HH:mm:ss"));

    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Database", "Insert failed:\n" + q.lastError().text());
        return;
    }

    models::SaleRecord rec;
    rec.id        = q.lastInsertId().toInt();
    rec.buyerName = m_buyerName.trimmed();
    rec.item      = item;
    rec.price     = price;
    rec.timestamp = now;

    m_records.append(rec);
    emit recordsChanged();

    appendLog(QString("[%1] #%2 | %3 | %4 | Rp %5")
        .arg(now.toString("HH:mm:ss"))
        .arg(rec.id)
        .arg(rec.item)
        .arg(rec.buyerName)
        .arg(rec.price));

    setBuyerName("");
}

void CashierViewModel::onRemoveRecord(int index) {
    if (index < 0 || index >= m_records.size()) return;

    const auto& rec = m_records.at(index);

    auto db = QSqlDatabase::database(DB_CONNECTION);
    QSqlQuery q(db);
    q.prepare("DELETE FROM sales WHERE id = ?");
    q.addBindValue(rec.id);
    q.exec();

    appendLog(QString("[%1] REMOVED #%2 | %3 | %4")
        .arg(QDateTime::currentDateTime().toString("HH:mm:ss"))
        .arg(rec.id)
        .arg(rec.item)
        .arg(rec.buyerName));

    m_records.removeAt(index);
    emit recordsChanged();
}

void CashierViewModel::onSelectExportPath() {
    const QString path = QFileDialog::getSaveFileName(
        nullptr, "Export Records", {}, "CSV Files (*.csv);;Text Files (*.txt)");
    if (!path.isEmpty()) {
        m_exportPath = path;
        emit exportPathChanged();
    }
}

void CashierViewModel::onExport() {
    if (m_exportPath.isEmpty()) {
        QMessageBox::warning(nullptr, "Export", "Please select a file path first.");
        return;
    }
    if (m_records.isEmpty()) {
        QMessageBox::warning(nullptr, "Export", "No records to export.");
        return;
    }

    QFile file(m_exportPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Export", "Cannot open file:\n" + m_exportPath);
        return;
    }

    QTextStream out(&file);
    out << "ID,Timestamp,Buyer,Item,Price\n";
    for (const auto& r : m_records) {
        out << r.id << ","
            << r.timestamp.toString("yyyy-MM-dd HH:mm:ss") << ","
            << r.buyerName << ","
            << r.item << ","
            << r.price << "\n";
    }

    appendLog(QString("[%1] Exported %2 records to %3")
        .arg(QDateTime::currentDateTime().toString("HH:mm:ss"))
        .arg(m_records.size())
        .arg(m_exportPath));

    QMessageBox::information(nullptr, "Export", "Exported successfully.");
}

int CashierViewModel::priceForIndex(int index) const {
    const QString item = m_availableItems.value(index);
    if (item == "Cheetos")    return 5000;
    if (item == "Cornetto")   return 8000;
    if (item == "Chocolatos") return 3000;
    return 0;
}

void CashierViewModel::appendLog(const QString& entry) {
    m_log.prepend(entry);
    emit logChanged();
}
