file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfSimplePeer SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfSimplePeer
    PUBLIC
    Qt5::Core
    Qt5::Qml
    VfEvent
    VfComponent
    VfNet2
    )

target_include_directories(VfSimplePeer
    PUBLIC
    $<INSTALL_INTERFACE:include/VfSimplePeer>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(VfSimplePeer PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfSimplePeer PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfSimplePeer)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfsimplepeer_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfSimplePeer
    )

export(TARGETS VfSimplePeer FILE VeinFrameworkTargets.cmake)
install(TARGETS VfSimplePeer
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfSimplePeer
    )
