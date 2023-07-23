file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfConvenientCode SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfConvenientCode
    PUBLIC
    Qt5::Core
    Qt5::Qml
    VfEvent
    VfComponent
    VfNet2
    )

target_include_directories(VfConvenientCode
    PUBLIC
    $<INSTALL_INTERFACE:include/VfConvenientCode>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

set_target_properties(VfConvenientCode PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfConvenientCode PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfConvenientCode)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfconvenientcode_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfConvenientCode
    )

export(TARGETS VfConvenientCode FILE VeinFrameworkTargets.cmake)
install(TARGETS VfConvenientCode
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfConvenientCode
    )