# Architecture

## Pattern: MVVM

This project follows the **Model-View-ViewModel (MVVM)** pattern, using C++ for the backend layer and QML for the UI layer. Qt's property binding and signal/slot mechanism provide the reactive glue.

```
┌──────────────────────────────────────────────────┐
│  VIEW  (QML)                                     │
│  Main.qml / LoginPage.qml / MainPage.qml         │
│  CashierTab.qml / AboutTab.qml                   │
│                                                  │
│  Binds to properties, calls slots/invokables     │
└──────────────────┬───────────────────────────────┘
                   │  Q_PROPERTY bindings
                   │  Signal → onXChanged
                   │  Slot / Q_INVOKABLE calls
┌──────────────────▼───────────────────────────────┐
│  VIEWMODEL  (C++ QObject singletons)             │
│  AuthViewModel   CashierViewModel   DeviceViewModel │
│                                                  │
│  Holds state, business logic, coordinates I/O   │
└──────────────────┬───────────────────────────────┘
                   │  QSqlQuery / QSerialPortInfo
┌──────────────────▼───────────────────────────────┐
│  MODEL / DATA                                    │
│  models::SaleRecord  (Models.h)                  │
│  SQLite: cashier.db                              │
└──────────────────────────────────────────────────┘
```

---

## Entry Point

`main.cpp` creates a `QGuiApplication` and a `QQmlApplicationEngine`, loads `qrc:/qml/Main.qml`, and enters the event loop. All three ViewModels are registered as singletons via `QML_SINGLETON` — the QML engine instantiates them automatically on first use.

---

## ViewModels

### AuthViewModel (`AuthVM`)

Manages authentication state.

| Member | Type | Description |
|---|---|---|
| `isLoggedIn` | `bool` (read) | Whether a user is authenticated |
| `errorMessage` | `QString` (read) | Last login error, empty on success |
| `onLogin(u, p)` | slot | Validates credentials, sets `isLoggedIn` |
| `onLogout()` | slot | Clears login state |

Credentials are hardcoded (`admin` / `admin`). Replace `onLogin` implementation for real auth.

---

### CashierViewModel (`CashierVM`)

Core business logic. Owns the SQLite database connection.

| Member | Type | Description |
|---|---|---|
| `buyerName` | `QString` (rw) | Current input buyer name |
| `selectedItemIndex` | `int` (rw) | Index into `availableItems` |
| `currentPrice` | `int` (read) | Derived from `selectedItemIndex` |
| `availableItems` | `QStringList` (const) | `["Cheetos", "Cornetto", "Chocolatos"]` |
| `records` | `QVariantList` (read) | All sales loaded from DB |
| `totalRecords` | `int` (read) | `records.size()` |
| `log` | `QStringList` (read) | Session activity log (prepended) |
| `exportPath` | `QString` (read) | Selected CSV export path |
| `onAddRecord()` | slot | Inserts sale into DB, updates lists |
| `onRemoveRecord(i)` | slot | Deletes sale by index |
| `onSelectExportPath()` | slot | Opens file-save dialog |
| `onExport()` | slot | Writes records to CSV |

**Database schema** (`cashier.db`, table `sales`):

```sql
CREATE TABLE sales (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    buyerName TEXT NOT NULL,
    item      TEXT NOT NULL,
    price     INTEGER NOT NULL,
    timestamp TEXT NOT NULL
);
```

Database is stored at `QStandardPaths::AppDataLocation/cashier.db` and auto-created on first run.

**Item pricing** (Indonesian Rupiah):

| Item | Price |
|---|---|
| Cheetos | 5,000 |
| Cornetto | 8,000 |
| Chocolatos | 3,000 |

---

### DeviceViewModel (`DeviceVM`)

Serial port enumeration.

| Member | Type | Description |
|---|---|---|
| `availablePorts` | `QStringList` (read) | `"portName — description"` per port |
| `onRefreshPorts()` | slot | Re-scans `QSerialPortInfo::availablePorts()` |

Only emits `availablePortsChanged` when the list actually changes.

---

## QML Component Tree

```
Main.qml (ApplicationWindow, 900×700)
│  StackLayout — switches on AuthVM.isLoggedIn
├── LoginPage.qml
│     Username/password form → AuthVM.onLogin()
│     Error label ← AuthVM.errorMessage
│
└── MainPage.qml
      TabBar: [Cashier] [About]
      StackLayout
      ├── CashierTab.qml
      │     New-sale row (buyer, item, price, add)
      │     Records table (ListView ← CashierVM.records)
      │     Session log (ListView ← CashierVM.log)
      │     Export panel (path selector + CSV button)
      │
      └── AboutTab.qml
            App info labels
            COM port ComboBox ← DeviceVM.availablePorts
            Refresh button → DeviceVM.onRefreshPorts()
```

---

## Data Flow — Adding a Sale

```
User fills buyer name + selects item + clicks Add
        │
        ▼
CashierTab: CashierVM.onAddRecord()
        │
        ├─ validate buyerName ≠ ""
        ├─ INSERT INTO sales (buyerName, item, price, timestamp)
        ├─ reload records from DB → emit recordsChanged
        ├─ appendLog("Added: ...") → emit logChanged
        └─ clear buyerName → emit buyerNameChanged
        │
        ▼
Records ListView auto-updates (model binding)
Log ListView auto-updates (model binding)
```

---

## Resource System

QML files are embedded via Qt's resource system (`qml.qrc`), accessible at `qrc:/qml/...`. The QML module is named `CashierApp` (version 1.0) — import it in QML with `import CashierApp`.

---

## Build Configurations

Defined in `CMakePresets.json`:

| Preset | Compiler | Type | Output dir |
|---|---|---|---|
| `debug-mingw` | MinGW | Debug | `build-debug-mingw/` |
| `release-mingw` | MinGW | Release | `build-release-mingw/` |
| `profile-mingw` | MinGW | RelWithDebInfo | `build-profile-mingw/` |
| `debug-mingw-makefiles` | MinGW | Debug (Makefiles) | `build-debug-mingw-makefiles/` |

C++ standard: **C++23**.  Qt requirement: **6.8+**.
