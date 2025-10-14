set(SOURCES
        Logger.cpp
        NetworkClient.cpp
        SettingsManager.cpp
        PlatformUtils.cpp
)

set(HEADERS
        Logger.h
        NetworkClient.h
        SettingsManager.h
        PlatformUtils.h
)

add_library(infrastructure ${SOURCES} ${HEADERS})
target_link_libraries(infrastructure PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Gui
)
target_include_directories(infrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})