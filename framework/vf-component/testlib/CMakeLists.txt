file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfComponentTest STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfComponentTest
    PUBLIC
    Qt5::Core
    VfComponent
    VfEvent
    VfStorageHash
    )

target_include_directories(VfComponentTest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(VfComponentTest PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfComponentTest PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfComponentTest)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcomponenttest_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponentTest
    )
