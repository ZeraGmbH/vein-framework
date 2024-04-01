file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfFrameworkTest STATIC
    ${SOURCES}
    ${HEADER}
)

target_link_libraries(VfFrameworkTest
    PUBLIC
    Qt5::Core
    VfEvent
    VfTcp
    VfConvenientCode
    veinserver-testlib
    )

target_include_directories(VfFrameworkTest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(VfFrameworkTest PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfFrameworkTest PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
