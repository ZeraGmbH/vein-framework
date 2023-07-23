file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfTcpTest STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfTcpTest
    PUBLIC
    Qt5::Core
    VfTcp
    )

target_include_directories(VfTcpTest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(VfTcpTest PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfTcpTest PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfTcpTest)

