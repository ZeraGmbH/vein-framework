# generate ecs_schema_generated from ecs_schema.fbs with flatbuffer
execute_process(COMMAND flatc --cpp ${CMAKE_CURRENT_SOURCE_DIR}/ecs_schema.fbs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfNet2 SHARED
    ${SOURCES}
    ${HEADER}
    )

target_include_directories(VfNet2
    PUBLIC
    $<INSTALL_INTERFACE:include/VfNet2>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(VfNet2
    PUBLIC
    Qt5::Core
    Qt5::Network
    flatbuffers
    VfHelpers
    VfComponent
    VfEvent
    VeinMeta::VfTcp
    )

set_target_properties(VfNet2 PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfNet2 PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfNet2)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfnet2_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfNet2
    )

export(TARGETS VfNet2 FILE VeinFrameworkTargets.cmake)
install(TARGETS VfNet2
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfNet2
    )

