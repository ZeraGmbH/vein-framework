# Dumps dummy target to see them in IDE
file(GLOB_RECURSE TESTFILES
    test-data/*
)
# Unique target name!
add_custom_target(vf-cpp-test-data ALL
    SOURCES ${TESTFILES}
    )

function(SETUP_VF_CPP_TESTS)
    SETUP_TESTS(${ARGN})
    foreach(_testname ${ARGN})
        target_compile_definitions(${_testname}
            PRIVATE
            TEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endforeach()
endfunction()

SETUP_VF_CPP_TESTS(
    test_rpc_signature
    test_rpc_simplified
)
