file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfStorage SHARED
    ${SOURCES}
    ${HEADER}
    )

target_include_directories(VfStorage
    PUBLIC
    $<INSTALL_INTERFACE:include/VfStorage>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

target_link_libraries(VfStorage
    PUBLIC
    ${QTX}::Core
    VfHelpers
    VfComponent
    VfEvent
    )

set_target_properties(VfStorage PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfStorage PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfStorage)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfstorage_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorage
    )

export(TARGETS VfStorage FILE VeinFrameworkTargets.cmake)
install(TARGETS VfStorage
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfStorage
    )
