file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(ModmanBase SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(ModmanBase 
    PUBLIC
    Qt5::Core
    VfEvent
    VfComponent
    )

set_target_properties(ModmanBase PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ModmanBase PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(ModmanBase)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/modmanbase_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ModmanBase
    )

target_include_directories(ModmanBase
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ModmanBase>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

export(TARGETS ModmanBase FILE VeinFrameworkTargets.cmake)
install(TARGETS ModmanBase
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ModmanBase
    )

