file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfComponent SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfComponent 
    PUBLIC
    Qt5::Core
    VfHelpers
    VfEvent
    )

set_target_properties(VfComponent PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfComponent PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfComponent)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcomponent_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponent
    )

target_include_directories(VfComponent
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfComponent>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

install(TARGETS VfComponent
    EXPORT VfComponentExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfComponentConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfComponentConfig.cmake.in VfComponentConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfComponentConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfComponentConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfComponent
    )

install(EXPORT VfComponentExport
    FILE VfComponentTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfComponent
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponent
    )
