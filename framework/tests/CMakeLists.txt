SETUP_TESTS(
    test_command_events
    test_test_vein_server
)

target_sources(test_command_events
  PRIVATE
    test-data/test-data.qrc
)

target_sources(test_test_vein_server
  PRIVATE
    test-data/test-data.qrc
)
