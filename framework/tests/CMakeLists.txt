SETUP_TESTS(
    test_storage
    test_command_events
)

target_sources(test_storage
  PRIVATE
    test-data/test-data.qrc
)

target_sources(test_command_events
  PRIVATE
    test-data/test-data.qrc
)
