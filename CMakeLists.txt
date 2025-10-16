cmake_minimum_required(VERSION 3.31)
set(PROJECT_NAME AtmoSense)
set(PROJECT_VERSION 0.1.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# set c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network Test)

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# additional options
option(ATMO_SENSE_ENABLE_TESTS "Enable tests" ON)

add_subdirectory(src)

if (ATMO_SENSE_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(AtmoSense PROPERTIES WIN32_EXECUTABLE FALSE)
else ()
    set_target_properties(AtmoSense PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()