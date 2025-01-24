file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(xsession-change-lib SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(xsession-change-lib 
    PUBLIC
    Qt5::Core
    )

set_target_properties(xsession-change-lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(xsession-change-lib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(xsession-change-lib
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xsession-change-lib>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

export(TARGETS xsession-change-lib FILE VeinFrameworkTargets.cmake)
install(TARGETS xsession-change-lib
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xsession-change-lib
    )


