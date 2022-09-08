file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfComponentTestListener SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfComponentTestListener
    PUBLIC
    Qt5::Core
    VfHelpers
    VfComponent
    VfEvent
    VfStorageHash
    )

target_include_directories(VfComponentTestListener
    PUBLIC
    $<INSTALL_INTERFACE:include/VfComponentTestListener>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(VfComponentTestListener PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfComponentTestListener PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfComponentTestListener)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcomponenttestlistener_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponentTestListener
    )

install(TARGETS VfComponentTestListener
    EXPORT VfComponentTestListenerExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfComponentTestListenerConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfComponentTestListenerConfig.cmake.in VfComponentTestListenerConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfComponentTestListenerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfComponentTestListenerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfComponentTestListener
    )

install(EXPORT VfComponentTestListenerExport
    FILE VfComponentTestListenerTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfComponentTestListener
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponentTestListener
    )
