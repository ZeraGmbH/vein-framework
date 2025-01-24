file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfFrameworkTest STATIC
    ${SOURCES}
    ${HEADER}
)

target_link_libraries(VfFrameworkTest
    PUBLIC
    Qt5::Core
    VfEvent
    VfTcp
    VfConvenientCode
    Zera::testlib-common
    vf-tcp-mock-networklib
    veinserver-testlib
    xsession-change-demolib
    )

target_include_directories(VfFrameworkTest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfFrameworkTest>
    )

set_target_properties(VfFrameworkTest PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfFrameworkTest PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS VfFrameworkTest NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS VfFrameworkTest
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfFrameworkTest
    )
