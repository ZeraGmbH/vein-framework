file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfComponentTest SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfComponentTest
    PUBLIC
    Qt5::Core
    VfHelpers
    VfComponent
    VfEvent
    VfStorageHash
    )

target_include_directories(VfComponentTest
    PUBLIC
    $<INSTALL_INTERFACE:include/VfComponentTest>
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

install(TARGETS VfComponentTest
    EXPORT VfComponentTestExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfComponentTestConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfComponentTestConfig.cmake.in VfComponentTestConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfComponentTestConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfComponentTestConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfComponentTest
    )

install(EXPORT VfComponentTestExport
    FILE VfComponentTestTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfComponentTest
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponentTest
    )
