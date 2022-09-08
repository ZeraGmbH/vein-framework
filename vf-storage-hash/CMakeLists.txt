file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfStorageHash SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfStorageHash 
    PUBLIC
    Qt5::Core
    VfHelpers
    VfComponent
    VfEvent
    )

target_include_directories(VfStorageHash
    PUBLIC
    $<INSTALL_INTERFACE:include/VfStorageHash>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(VfStorageHash PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfStorageHash PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfStorageHash)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfstoragehash_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorageHash
    )

install(TARGETS VfStorageHash
    EXPORT VfStorageHashExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

write_basic_package_version_file(
    VfStorageHashConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VfStorageHashConfig.cmake.in VfStorageHashConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VfStorageHashConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VfStorageHashConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfStorageHash
    )

install(EXPORT VfStorageHashExport
    FILE VfStorageHashTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VfStorageHash
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorageHash
    )
