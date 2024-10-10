file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfCpp SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfCpp 
    PUBLIC
    Qt5::Core
    VfEvent
    VfComponent
    VfConvenientCode
    )

set_target_properties(VfCpp PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfCpp PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfCpp)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcpp_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfCpp
    )

target_include_directories(VfCpp
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfCpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

export(TARGETS VfCpp FILE VeinFrameworkTargets.cmake)
install(TARGETS VfCpp
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfCpp
    )

