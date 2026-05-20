cmake_minimum_required(VERSION 3.16)
project(qt-qml-project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_QML_GENERATE_QMLLS_INI ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Quick Widgets Sql SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick Widgets Sql SerialPort)

qt_standard_project_setup(REQUIRES 6.8)

qt6_add_resources(QML_RESOURCES qml.qrc)

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Models.h
    src/AuthViewModel.h    src/AuthViewModel.cpp
    src/CashierViewModel.h src/CashierViewModel.cpp
    src/DeviceViewModel.h  src/DeviceViewModel.cpp
    ${QML_RESOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::SerialPort
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
