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

find_package(Qt5 COMPONENTS Core CONFIG REQUIRED)

add_subdirectory(vf-helpers)
add_subdirectory(vf-event)
add_subdirectory(vf-component)
add_subdirectory(vf-storage-hash)
add_subdirectory(vf-test-helpers)

if(NOT CMAKE_CROSSCOMPILING)
    enable_testing()
    #add_subdirectory(tests)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
