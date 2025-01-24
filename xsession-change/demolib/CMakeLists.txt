file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(xsession-change-demolib STATIC
    ${SOURCES}
    ${HEADER}
)

target_include_directories(xsession-change-demolib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xsession-change-demolib>
    )

target_link_libraries(xsession-change-demolib
    PUBLIC
    xsession-change-lib
)

set_target_properties(xsession-change-demolib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(xsession-change-demolib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS xsession-change-demolib NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS xsession-change-demolib
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xsession-change-demolib
    )
