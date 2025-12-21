file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(vf-tcp-mock-networklib STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(vf-tcp-mock-networklib
    PUBLIC
    ${QTX}::Core
    VfTcp
    )

target_include_directories(vf-tcp-mock-networklib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vf-tcp-mock-networklib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(vf-tcp-mock-networklib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vf-tcp-mock-networklib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(vf-tcp-mock-networklib)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vf-tcp-mock-networklib_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vf-tcp-mock-networklib
    )

export(TARGETS vf-tcp-mock-networklib NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS vf-tcp-mock-networklib
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vf-tcp-mock-networklib
    )
