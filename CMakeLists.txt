cmake_minimum_required(VERSION 3.23)
project(MCSandbox)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH D:/Programming/libs/CPP/Qt/Qt5.14.2/5.14.2/mingw73_64/lib/cmake/Qt5) # path/to/Qt5

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build) # path/to/build_directory

find_library(WSOCK32_LIBRARY wsock32)
find_library(WS2_32_LIBRARY ws2_32)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        PrintSupport
        REQUIRED
)

add_executable(
        ${PROJECT_NAME}
        WIN32
        source/main.cpp
        headers/mainwindow.h
        source/mainwindow.cpp
        ui/mainwindow.ui
        headers/dialog.h
        source/dialog.cpp
        ui/dialog.ui
        headers/bot.h
        headers/fplib.h
        source/fplib.cpp
        headers/realonline.h
        source/realonline.cpp
)

target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)