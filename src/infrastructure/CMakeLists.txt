set(SOURCES
        Logger.cpp
        NetworkClient.cpp
        SettingsManager.cpp
        PlatformUtils.cpp
        ThemeManager.cpp
        TranslationManager.cpp
        UnitFormatter.cpp

        notifications/NotificationSoundPlayer.cpp
)

set(HEADERS
        Logger.h
        NetworkClient.h
        SettingsManager.h
        PlatformUtils.h
        ThemeManager.h
        TranslationManager.h
        UnitFormatter.h

        notifications/NotificationSoundPlayer.h
)

add_library(infrastructure ${SOURCES} ${HEADERS})
target_link_libraries(infrastructure PRIVATE
        domain
        data

        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Multimedia
)

target_link_libraries(infrastructure PRIVATE spdlog)

target_include_directories(infrastructure PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/open_meteo
        ${CMAKE_CURRENT_SOURCE_DIR}/open_weather_map
)

target_sources(infrastructure PRIVATE
        factory/WeatherProviderFactory.h
        factory/WeatherProviderFactory.cpp
        factory/GeocoderFactory.h
        factory/GeocoderFactory.cpp
)