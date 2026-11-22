# Dumps dummy target to see them in IDE
file(GLOB_RECURSE TESTFILES
    test-data/*
)
# Unique target name!
add_custom_target(framework-test-data ALL
    SOURCES ${TESTFILES}
    )

function(SETUP_FRAMEWORK_TESTS)
    SETUP_TESTS(${ARGN})
    foreach(_testname ${ARGN})
        target_compile_definitions(${_testname}
            PRIVATE
            TEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endforeach()
endfunction()


SETUP_FRAMEWORK_TESTS(
    test_command_events
    test_test_vein_server
)

target_link_libraries(test_test_vein_server
  PUBLIC
    xsession-change-demolib
)
