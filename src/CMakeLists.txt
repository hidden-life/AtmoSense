# domain
add_subdirectory(domain)

# infrastructure
add_subdirectory(infrastructure)

# data (providers, cache etc.) that depends on domain + infrastructure
add_subdirectory(data)

# UI
add_subdirectory(ui)

# main app
set(SOURCES
        application/main.cpp

        application/Application.cpp
        application/TrayService.cpp
        application/IconMapper.cpp

        core/ApplicationContext.cpp
)

set(HEADERS
        application/Application.h
        application/TrayService.h
        application/IconMapper.h

        core/ApplicationContext.h
)

set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

set(RESOURCES
    ${RESOURCES_DIR}/resources.qrc
)

add_executable(AtmoSense ${SOURCES} ${HEADERS} ${RESOURCES})

target_link_libraries(AtmoSense PRIVATE
        domain
        infrastructure
        data
        ui

        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
)

target_include_directories(AtmoSense PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(AtmoSense PROPERTIES WIN32_EXECUTABLE TRUE MACOSX_BUNDLE FALSE)