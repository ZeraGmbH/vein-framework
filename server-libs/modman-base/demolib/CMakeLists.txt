file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(veinserver-demolib STATIC
    ${SOURCES}
    ${HEADER}
)

target_include_directories(veinserver-demolib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/veinserver-demolib>
    )

target_link_libraries(veinserver-demolib
    PUBLIC
    ModmanBase
)

set_target_properties(veinserver-demolib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(veinserver-demolib PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS veinserver-demolib NAMESPACE Zera:: FILE VeinFrameworkTargets.cmake)
install(TARGETS veinserver-demolib
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/veinserver-demolib
    )
