file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfEvent SHARED
    ${SOURCES}
    ${HEADER}
    )

target_include_directories(VfEvent
    PUBLIC
    $<INSTALL_INTERFACE:include/VfEvent>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

target_link_libraries(VfEvent 
    PUBLIC
    Qt5::Core
    VfHelpers
    Zera::zera-timers
    )

set_target_properties(VfEvent PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfEvent PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfEvent)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfevent_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfEvent
    )

export(TARGETS VfEvent FILE VeinFrameworkTargets.cmake)
install(TARGETS VfEvent
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfEvent
    )
