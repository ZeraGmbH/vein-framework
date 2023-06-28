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

target_include_directories(VfStorageHash
    PUBLIC
    $<INSTALL_INTERFACE:include/VfStorageHash>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

target_link_libraries(VfStorageHash
    PUBLIC
    Qt5::Core
    VfHelpers
    VfComponent
    VfEvent
    )

set_target_properties(VfStorageHash PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfStorageHash PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfStorageHash)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfstoragehash_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorageHash
    )

export(TARGETS VfStorageHash FILE VeinFrameworkTargets.cmake)
install(TARGETS VfStorageHash
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorageHash
    )
