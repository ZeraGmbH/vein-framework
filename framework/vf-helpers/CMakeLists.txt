file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfHelpers STATIC
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
    ${QTX}::Core
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
