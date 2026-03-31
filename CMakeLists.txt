cmake_minimum_required(VERSION 3.19)
project(RioClipboardManager LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

qt_add_executable(RioClipboardManager
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/quickmenu.cpp
    src/quickmenu.h
    ui/quickmenu.ui
    src/clipboardmanager.h src/clipboardmanager.cpp
    src/traycontroller.h src/traycontroller.cpp
    src/Structs.h
    AppResources.qrc
    src/clipboardmodel.h src/clipboardmodel.cpp
    src/listitemdelegate.h src/listitemdelegate.cpp
    database/databasemanager.h database/databasemanager.cpp
    src/settingswindow.h src/settingswindow.cpp ui/settingswindow.ui
    src/appsettings.h src/appsettings.cpp
    src/hotkeyedit.h src/hotkeyedit.cpp
    resources.rc

)

target_link_libraries(RioClipboardManager
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Sql
)

target_include_directories(RioClipboardManager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/database
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(GNUInstallDirs)

install(TARGETS RioClipboardManager
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET RioClipboardManager
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
