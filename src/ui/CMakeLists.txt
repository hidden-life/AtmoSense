set(SOURCES
        MainWindow.cpp
        # dialogs
        SettingsDialog.cpp
        LocationSearchDialog.cpp

        # widgets
        WeatherSummaryWidget.cpp
        DailyForecastWidget.cpp
        HourlyForecastWidget.cpp
)

set(HEADERS
        MainWindow.h
        # dialogs
        SettingsDialog.h
        LocationSearchDialog.h

        # widgets
        WeatherSummaryWidget.h
        DailyForecastWidget.h
        HourlyForecastWidget.h
)

set(UI_FILES
        MainWindow.ui
        SettingsDialog.ui
        LocationSearchDialog.ui
        WeatherSummaryWidget.ui
        DailyForecastWidget.ui
        HourlyForecastWidget.ui
)

add_library(ui ${SOURCES} ${HEADERS} ${UI_FILES})

target_link_libraries(ui PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets

        domain
        infrastructure
)

target_include_directories(ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/core
        ${CMAKE_SOURCE_DIR}/src
)
#target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/core)
#target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src)