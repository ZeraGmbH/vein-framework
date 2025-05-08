file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(vfcpp-testlib STATIC
    ${SOURCES}
    ${HEADER}
)

target_include_directories(vfcpp-testlib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(vfcpp-testlib
    PUBLIC
    VfCpp
)

set_target_properties(vfcpp-testlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vfcpp-testlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

