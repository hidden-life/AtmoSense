set(SOURCES
        Logger.cpp
        NetworkClient.cpp
        SettingsManager.cpp
        PlatformUtils.cpp
        ThemeManager.cpp
        TranslationManager.cpp
)

set(HEADERS
        Logger.h
        NetworkClient.h
        SettingsManager.h
        PlatformUtils.h
        ThemeManager.h
        TranslationManager.h
)

add_library(infrastructure ${SOURCES} ${HEADERS})
target_link_libraries(infrastructure PRIVATE
        domain
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)
target_include_directories(infrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})