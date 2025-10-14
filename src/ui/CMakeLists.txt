set(SOURCES
    MainWindow.cpp
)

set(HEADERS
    MainWindow.h
)

set(UI_FILES
    MainWindow.ui
)

add_library(ui ${SOURCES} ${HEADERS} ${UI_FILES})

target_link_libraries(ui PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets

        domain
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})