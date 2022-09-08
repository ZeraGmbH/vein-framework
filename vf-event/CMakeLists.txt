file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfEvent SHARED
    ${SOURCES}
    ${HEADER}
    )


target_link_libraries(VfEvent 
    PUBLIC
    Qt5::Core
    VfHelpers
    )

set_target_properties(VfEvent PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfEvent PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfEvent)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfevent_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfEvent
    )

target_include_directories(VfEvent
    PUBLIC
    $<INSTALL_INTERFACE:include/VfEvent>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

install(TARGETS VfEvent
    EXPORT VfEventExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfEventConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfEventConfig.cmake.in VfEventConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfEventConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfEventConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfEvent
    )

install(EXPORT VfEventExport
    FILE VfEventTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfEvent
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfEvent
    )
