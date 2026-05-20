# CLAUDE.md

## Project Overview

Qt 6 / QML MVVM template application — a simple cashier/POS app used as a boilerplate for QML desktop development. The full architecture is documented in `architecture.md`.

## Build System

- **CMake 3.16+** with presets defined in `CMakePresets.json`
- **Toolchain**: MinGW (g++) with Ninja generator
- **Qt version**: 6.8+ required

```powershell
# Configure and build (debug)
cmake --preset debug-mingw
cmake --build build-debug-mingw

# Run
./build-debug-mingw/qt_qml_project.exe
```

Available presets: `debug-mingw`, `release-mingw`, `profile-mingw`, `debug-mingw-makefiles`

## Project Structure

```
src/         C++ ViewModels and data models
qml/         QML UI components
qml/tabs/    Tab-level QML pages
```

## Architecture: MVVM

Every feature follows a strict MVVM separation:

- **Model**: Plain C++ structs in `src/Models.h`
- **ViewModel**: `QObject` subclass in `src/`, registered as `QML_SINGLETON`
- **View**: `.qml` file in `qml/` — no business logic, binds to ViewModel properties

### Adding a new ViewModel

1. Create `src/FooViewModel.h` / `src/FooViewModel.cpp`
2. Inherit `QObject`, add `Q_OBJECT`, `QML_ELEMENT`, `QML_SINGLETON`
3. Expose state via `Q_PROPERTY` with `NOTIFY` signals
4. Expose actions via `public slots:` or `Q_INVOKABLE`
5. Add the `.h`/`.cpp` pair to the `qt_add_qml_module` sources in `CMakeLists.txt`
6. Use as `FooVM.property` in QML (singleton, no instantiation needed)

### Adding a new QML page

1. Create `qml/NewPage.qml`
2. Add it to `qt_add_qml_module(QML_FILES ...)` in `CMakeLists.txt`
3. Add it to `qml.qrc`
4. Import the module: `import CashierApp`

## Key Conventions

- ViewModels are **singletons** — one instance per class, no manual `new`
- All ViewModel properties are read-only from QML; mutations go through slots/invokables
- Log entries are **prepended** (most recent first)
- Currency is stored as `int` in Indonesian Rupiah (no decimals)
- Database is SQLite at `QStandardPaths::AppDataLocation/cashier.db`

## Qt Modules Used

| Module | Purpose |
|---|---|
| Qt6::Quick | QML runtime |
| Qt6::Widgets | QFileDialog, QMessageBox |
| Qt6::Sql | SQLite persistence |
| Qt6::SerialPort | COM port enumeration |

## QML Language Server

`.qmlls.ini` points to `build-debug-mingw/` — keep a debug build present for IDE features to work.

## Hardcoded Credentials (dev only)

Username: `admin` / Password: `admin` — replace `AuthViewModel::onLogin` for any real deployment.
