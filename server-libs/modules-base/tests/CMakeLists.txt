# Dumps dummy target to see them in IDE
file(GLOB_RECURSE TESTFILES
    test-data/*
)
# Unique target name!
add_custom_target(module-base-test-data ALL
    SOURCES ${TESTFILES}
    )

function(SETUP_MODMANBASE_TESTS)
    foreach(_testname ${ARGN})
        SETUP_TESTS(${_testname})
        target_link_libraries(${_testname}
            PUBLIC
            modules-vein
        )
        target_compile_definitions(${_testname}
            PRIVATE
            TEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endforeach()
endfunction()

SETUP_MODMANBASE_TESTS(
    test_json_param_validator
    )
