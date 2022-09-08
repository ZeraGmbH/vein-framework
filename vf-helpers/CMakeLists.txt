file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfHelpers SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfHelpers 
    PRIVATE
    Qt5::Core
    )

target_include_directories(VfHelpers 
    PUBLIC
    $<INSTALL_INTERFACE:include/VfHelpers>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(VfHelpers PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfHelpers PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfHelpers)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfhelpers_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfHelpers
    )

install(TARGETS VfHelpers
    EXPORT VfHelpersExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfHelpersConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfHelpersConfig.cmake.in VfHelpersConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfHelpersConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfHelpersConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfHelpers
    )

install(EXPORT VfHelpersExport
    FILE VfHelpersTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfHelpers
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfHelpers
    )

