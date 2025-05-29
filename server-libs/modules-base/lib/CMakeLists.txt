file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(modules-vein SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(modules-vein
    PUBLIC
    Qt5::Core
    zera-json-params
    # ask linker to help us finding unresolved symbols
    "-Wl,--no-undefined"
    )

target_include_directories(modules-vein
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vein-framework/modules-vein>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(modules-vein PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(modules-vein PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(modules-vein)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/modules-vein_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vein-framework/modules-vein
    )

export(TARGETS modules-vein NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS modules-vein
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vein-framework/modules-vein
    )
