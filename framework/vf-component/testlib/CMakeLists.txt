file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfComponentTest STATIC
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfComponentTest
    PUBLIC
    ${QTX}::Core
    VfComponent
    VfEvent
    VfStorage
    VfConvenientCode
    )

target_include_directories(VfComponentTest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/VfComponentTest>
    )

set_target_properties(VfComponentTest PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfComponentTest PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS VfComponentTest NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS VfComponentTest
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfComponentTest
    )
