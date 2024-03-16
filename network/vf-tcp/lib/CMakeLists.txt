file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
    )

add_library(VfTcp SHARED
    ${SOURCES}
    ${HEADER}
)

target_include_directories(VfTcp
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfTcp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(VfTcp 
    PUBLIC
    Qt5::Core
    Qt5::Network
    systemd
    )

set_target_properties(VfTcp PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfTcp PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfTcp)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vftcp_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfTcp
    )

export(TARGETS VfTcp FILE VeinFrameworkTargets.cmake)
install(TARGETS VfTcp
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfTcp
    )

