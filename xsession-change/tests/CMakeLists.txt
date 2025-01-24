function(SETUP_XSESSION_CHANGE_TESTS)
    foreach(_testname ${ARGN})
        SETUP_TESTS(${_testname})
        target_link_libraries(${_testname}
            PUBLIC
            ModmanBase
            xsession-change-demolib
        )
    endforeach()
endfunction()

SETUP_XSESSION_CHANGE_TESTS(
    test_lxdm_config_file
    )

