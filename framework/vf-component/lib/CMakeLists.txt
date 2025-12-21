file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfComponent SHARED
    ${SOURCES}
    ${HEADER}
    )

target_include_directories(VfComponent
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfComponent>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

target_link_libraries(VfComponent
    PUBLIC
    ${QTX}::Core
    VfHelpers
    VfEvent
    )

set_target_properties(VfComponent PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfComponent PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfComponent)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcomponent_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponent
    )

export(TARGETS VfComponent FILE VeinFrameworkTargets.cmake)
install(TARGETS VfComponent
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponent
    )
