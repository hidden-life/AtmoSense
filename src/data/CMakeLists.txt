set(SOURCES
        cache/FileCacheStore.cpp

        repository/WeatherRepository.cpp

        open_meteo/OpenMeteoGeocoder.cpp
        open_meteo/OpenMeteoWeatherProvider.cpp

        open_weather_map/OpenWeatherMapWeatherProvider.cpp
)

set(HEADERS
        cache/FileCacheStore.h

        repository/WeatherRepository.h

        open_meteo/OpenMeteoGeocoder.h
        open_meteo/OpenMeteoWeatherProvider.h

        open_weather_map/OpenWeatherMapWeatherProvider.h
)

add_library(data ${SOURCES} ${HEADERS})
target_link_libraries(data PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network

        domain
        infrastructure
)

target_link_libraries(data PRIVATE spdlog)

target_include_directories(data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})