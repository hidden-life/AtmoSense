add_library(domain INTERFACE)

target_include_directories(domain INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(domain PRIVATE model/Provider.h)