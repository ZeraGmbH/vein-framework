file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(xiqnet SHARED
    ${SOURCES}
    ${HEADER}
    )

target_include_directories(xiqnet
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xiqnet>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(xiqnet 
    PUBLIC
    ${QTX}::Core
    ${QTX}::Network
    ${Protobuf_LIBRARIES}   
    zeraprotobuf
    VfTcp
    )

set_target_properties(xiqnet PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(xiqnet PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(xiqnet)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xiqnet_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xiqnet
    )

export(TARGETS xiqnet FILE VeinFrameworkTargets.cmake)
install(TARGETS xiqnet
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xiqnet
    )

