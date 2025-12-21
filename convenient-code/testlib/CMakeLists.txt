file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfConvenientCodeTest STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfConvenientCodeTest
    PUBLIC
    ${QTX}::Core
    VfEvent
    VfTcp
    VfConvenientCode
    veinserver-testlib
    )

target_include_directories(VfConvenientCodeTest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfConvenientCodeTest>
    )

set_target_properties(VfConvenientCodeTest PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfConvenientCodeTest PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS VfConvenientCodeTest NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS VfConvenientCodeTest
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfConvenientCodeTest
    )
