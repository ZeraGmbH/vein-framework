add_subdirectory(lib)

if(NOT SKIP_UNITTESTS)
    add_subdirectory(tests)
    add_subdirectory(testlib)
endif()
