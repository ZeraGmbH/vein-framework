set(SOURCE_PROTO ${CMAKE_CURRENT_SOURCE_DIR}/netmessages.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${SOURCE_PROTO})

add_library(zeraprotobuf SHARED
    ${PROTO_HDRS}
    ${PROTO_SRCS}
    # just for qt-creator display
    ${SOURCE_PROTO}
    )

target_link_libraries(zeraprotobuf
    PRIVATE
    ${Protobuf_LIBRARIES}
    )

target_include_directories(zeraprotobuf
    PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/zeraprotobuf>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

set_target_properties(zeraprotobuf PROPERTIES OUTPUT_NAME "zera-protobuf")
set_target_properties(zeraprotobuf PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(zeraprotobuf PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

export(TARGETS zeraprotobuf FILE VeinFrameworkTargets.cmake)
install(TARGETS zeraprotobuf
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${PROTO_HDRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zeraprotobuf
    )

