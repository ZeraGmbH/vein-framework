file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfHelpers SHARED
    ${SOURCES}
    ${HEADER}
    )


target_include_directories(VfHelpers 
    PUBLIC
    $<INSTALL_INTERFACE:include/VfHelpers>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

target_link_libraries(VfHelpers
    PUBLIC
    Qt5::Core
    )

set_target_properties(VfHelpers PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfHelpers PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfHelpers)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfhelpers_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfHelpers
    )

export(TARGETS VfHelpers FILE VeinFrameworkTargets.cmake)
install(TARGETS VfHelpers
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfHelpers
    )
