#pragma once
#include <QObject>
#include <QStringList>
#include <QVariantList>
#include <QList>
#include "Models.h"

class CashierViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString      buyerName         READ buyerName         WRITE setBuyerName         NOTIFY buyerNameChanged)
    Q_PROPERTY(int          selectedItemIndex READ selectedItemIndex WRITE setSelectedItemIndex  NOTIFY selectedItemIndexChanged)
    Q_PROPERTY(int          currentPrice      READ currentPrice      NOTIFY currentPriceChanged)
    Q_PROPERTY(QStringList  availableItems    READ availableItems    CONSTANT)
    Q_PROPERTY(QVariantList records           READ recordsVariant    NOTIFY recordsChanged)
    Q_PROPERTY(int          totalRecords      READ totalRecords      NOTIFY recordsChanged)
    Q_PROPERTY(QStringList  log               READ log               NOTIFY logChanged)
    Q_PROPERTY(QString      exportPath        READ exportPath        NOTIFY exportPathChanged)

public:
    explicit CashierViewModel(QObject* parent = nullptr);
    ~CashierViewModel();

    QString      buyerName()         const { return m_buyerName; }
    int          selectedItemIndex() const { return m_selectedItemIndex; }
    int          currentPrice()      const;
    QStringList  availableItems()    const { return m_availableItems; }
    QVariantList recordsVariant()    const;
    int          totalRecords()      const { return m_records.size(); }
    QStringList  log()               const { return m_log; }
    QString      exportPath()        const { return m_exportPath; }

    Q_INVOKABLE void setBuyerName(const QString& v);
    Q_INVOKABLE void setSelectedItemIndex(int v);

signals:
    void buyerNameChanged();
    void selectedItemIndexChanged();
    void currentPriceChanged();
    void recordsChanged();
    void logChanged();
    void exportPathChanged();

public slots:
    void onAddRecord();
    void onRemoveRecord(int index);
    void onSelectExportPath();
    void onExport();

private:
    QString     m_buyerName;
    int         m_selectedItemIndex{0};
    QStringList m_availableItems;
    QList<models::SaleRecord> m_records;
    QStringList m_log;
    QString     m_exportPath;

    void initDb();
    void loadRecords();
    int  priceForIndex(int index) const;
    void appendLog(const QString& entry);
};
