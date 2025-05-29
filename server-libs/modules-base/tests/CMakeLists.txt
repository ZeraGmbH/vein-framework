function(SETUP_MODULESBASE_TESTS)
    foreach(_testname ${ARGN})
        SETUP_TESTS(${_testname})
        target_link_libraries(${_testname}
            PUBLIC
            modules-vein
        )
    endforeach()
endfunction()

SETUP_MODMANBASE_TESTS(
    test_json_param_validator
    )

target_sources(test_json_param_validator
  PRIVATE
    test-data/test-data.qrc
)
