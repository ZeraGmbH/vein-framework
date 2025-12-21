file(GLOB SOURCES
    *.cpp
    )

file(GLOB HEADER
    *.h
    )

add_library(vf-tcp-mock-testlib STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(vf-tcp-mock-testlib
    PUBLIC
    ${QTX}::Core
    VfTcp
    Zera::zera-timers
    )

target_include_directories(vf-tcp-mock-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(vf-tcp-mock-testlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vf-tcp-mock-testlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})


