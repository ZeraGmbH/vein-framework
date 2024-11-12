file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfQml SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfQml 
    PUBLIC
    Qt5::Core
    Qt5::Qml
    VfHelpers
    VfEvent
    VfComponent 
    )

target_include_directories(VfQml
    PUBLIC
    $<INSTALL_INTERFACE:include/VfQml>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(VfQml PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfQml PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfQml)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfqml_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfQml
    )

export(TARGETS VfQml FILE VeinFrameworkTargets.cmake)
install(TARGETS VfQml
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfQml
    )

