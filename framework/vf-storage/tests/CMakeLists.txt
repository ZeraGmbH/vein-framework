# Dumps dummy target to see them in IDE
file(GLOB_RECURSE TESTFILES
    test-data/*
)
# Unique target name!
add_custom_target(vf-storage-test-data ALL
    SOURCES ${TESTFILES}
    )

function(SETUP_VF_STORAGE_TESTS)
    SETUP_TESTS(${ARGN})
    foreach(_testname ${ARGN})
        target_compile_definitions(${_testname}
            PRIVATE
            TEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        )
    endforeach()
endfunction()

SETUP_VF_STORAGE_TESTS(
    test_storage
    test_storage_notification
    test_storage_future_notification
    test_client_storage_event_system
    test_storage_direct_write
    test_storage_recorder
)
