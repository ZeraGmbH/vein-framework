file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(vf-tcp-testlib STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(vf-tcp-testlib
    PUBLIC
    Qt5::Core
    VfTcp
    vf-tcp-mock-networklib
    )

target_include_directories(vf-tcp-testlib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vf-tcp-testlib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(vf-tcp-testlib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vf-tcp-testlib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(vf-tcp-testlib)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vf-tcp-testlib_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vf-tcp-testlib
    )

export(TARGETS vf-tcp-testlib NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS vf-tcp-testlib
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vf-tcp-testlib
    )
