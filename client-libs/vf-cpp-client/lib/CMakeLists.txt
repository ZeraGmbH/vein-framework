file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfCppClient SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfCppClient
    PUBLIC
    Qt5::Core
    Qt5::Qml
    VfHelpers
    VfEvent
    VfComponent
    VfNet2
    )

target_include_directories(VfCppClient
    PUBLIC
    $<INSTALL_INTERFACE:include/VfCppClient>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(VfCppClient PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfCppClient PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfCppClient)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcppclient_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfCppClient
    )

export(TARGETS VfCppClient FILE VeinFrameworkTargets.cmake)
install(TARGETS VfCppClient
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfCppClient
    )
