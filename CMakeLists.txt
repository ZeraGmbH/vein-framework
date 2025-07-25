cmake_minimum_required(VERSION 3.1)

project(vein-framework
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(FeatureSummary)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

option(useOpenSsl "Using OpenSSL" ON)

find_package(Qt5 COMPONENTS Core Network Qml Test CONFIG REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(Protobuf REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(zera-json-params REQUIRED)
find_package(SCPI REQUIRED)
pkg_check_modules(Systemd REQUIRED libsystemd)
if(useOpenSsl)
	find_package(OpenSSL COMPONENTS SSL Crypto REQUIRED)
endif()
find_package(ZenuxCore REQUIRED)

find_program(PLANTUML NAMES plantuml NO_CACHE)
if(${PLANTUML} MATCHES "PLANTUML-NOTFOUND")
    message(WARNING "Program plantuml not found. On Fedora install plantuml")
endif()

enable_testing()
# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt
function(SETUP_TESTS)
    foreach(_testname ${ARGN})
        add_test(NAME ${_testname}
                 COMMAND ${_testname})
        add_executable(${_testname}
            ${_testname}.h
            ${_testname}.cpp
            )
        target_include_directories(${_testname}
            PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            ${CMAKE_CURRENT_SOURCE_DIR}
            )
        target_link_libraries(${_testname}
            PUBLIC
            Qt5::Test
            VfComponentTest
            VfCpp-testlib
            VfNet2
            VfQml
            vf-tcp-mock-networklib
            vf-tcp-mock-testlib
            VfFrameworkTest
            VfConvenientCodeTest
            Zera::testlib-common
            Zera::zera-timers-testlib
            )
    endforeach()
endfunction()

write_basic_package_version_file(
    VeinFrameworkConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
    )

configure_file(VeinFrameworkConfig.cmake.in VeinFrameworkConfig.cmake @ONLY)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/VeinFrameworkConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VeinFrameworkConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VeinFramework
    )

install(EXPORT VeinFrameworkExport
    FILE VeinFrameworkTargets.cmake
    NAMESPACE Zera::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VeinFramework
    )

add_subdirectory(client-libs)
add_subdirectory(convenient-code)
add_subdirectory(framework)
add_subdirectory(network)
add_subdirectory(server-libs)
add_subdirectory(xsession-change)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
