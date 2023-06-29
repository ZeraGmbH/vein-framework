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

find_package(Qt5 COMPONENTS Core Network Test CONFIG REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(Protobuf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Systemd REQUIRED libsystemd)

enable_testing()

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

add_subdirectory(framework)
add_subdirectory(network)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
