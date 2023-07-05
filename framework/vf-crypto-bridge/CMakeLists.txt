file(GLOB SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(VfCryptoBridge SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(VfCryptoBridge 
    PUBLIC
    Qt5::Core
    )

if(${OpenSSL_FOUND})
    target_link_libraries(VfCryptoBridge 
        PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

set_target_properties(VfCryptoBridge PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(VfCryptoBridge PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

generate_export_header(VfCryptoBridge)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfcryptobridge_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfCryptoBridge
    )

target_include_directories(VfCryptoBridge
    PUBLIC
    $<INSTALL_INTERFACE:include/VfCryptoBridge>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

export(TARGETS VfCryptoBridge FILE VeinFrameworkTargets.cmake)
install(TARGETS VfCryptoBridge
    EXPORT VeinFrameworkExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VfCryptoBridge
    )

