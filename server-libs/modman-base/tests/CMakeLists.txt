function(SETUP_MODMANBASE_TESTS)
    foreach(_testname ${ARGN})
        SETUP_TESTS(${_testname})
        target_link_libraries(${_testname}
            PUBLIC
            ModmanBase
            veinserver-testlib
            xsession-change-demolib
        )
    endforeach()
endfunction()

SETUP_MODMANBASE_TESTS(
    test_modman_start
    )
