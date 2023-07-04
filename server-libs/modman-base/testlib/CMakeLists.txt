file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(veinserver-testlib STATIC
    ${SOURCES}
    ${HEADER}
)

target_include_directories(veinserver-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(veinserver-testlib
    PUBLIC
    VfComponentTest
    VfCpp
    VfNet2
    ModmanBase
)

set_target_properties(veinserver-testlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(veinserver-testlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
