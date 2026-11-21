cmake_minimum_required(VERSION 3.21)

project(PosCashierSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# This is the existing Qt installation used by CLion and local CMake builds.
set(POS_QT_DIR "$ENV{HOME}/.local/share/system-monitor-qt/6.8.3/macos"
    CACHE PATH "Qt 6 installation directory")
list(PREPEND CMAKE_PREFIX_PATH "${POS_QT_DIR}")

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Widgets Sql PrintSupport Test)

qt_standard_project_setup()

qt_add_executable(pos_cashier_system
    MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/database.cpp
    src/services/authservice.cpp
    src/services/productservice.cpp
    src/services/saleservice.cpp
    include/mainwindow.h
    include/database.h
    include/models/types.h
    include/services/authservice.h
    include/services/productservice.h
    include/services/saleservice.h
)

target_include_directories(pos_cashier_system PRIVATE include)
target_link_libraries(pos_cashier_system PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql Qt6::PrintSupport)

qt_add_resources(pos_cashier_system "app_resources"
    PREFIX "/"
    FILES assets/styles/dark.qss database/schema.sql
)

enable_testing()
qt_add_executable(pos_core_tests tests/core_tests.cpp src/database.cpp src/services/productservice.cpp src/services/saleservice.cpp include/database.h include/models/types.h include/services/productservice.h include/services/saleservice.h)
target_include_directories(pos_core_tests PRIVATE include)
target_link_libraries(pos_core_tests PRIVATE Qt6::Core Qt6::Sql Qt6::Test)
qt_add_resources(pos_core_tests "test_resources" PREFIX "/" FILES database/schema.sql)
add_test(NAME pos_core_tests COMMAND pos_core_tests)

set_target_properties(pos_cashier_system PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.poscashiersystem
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
)
