file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(validators SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(validators
    PUBLIC
    Qt5::Core
    zera-json-params
    # ask linker to help us finding unresolved symbols
    "-Wl,--no-undefined"
    )

target_include_directories(validators
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vein-framework/validators>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(validators PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(validators PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(validators)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/validators_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vein-framework/validators
    )

export(TARGETS validators NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS validators
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vein-framework/validators
    )
