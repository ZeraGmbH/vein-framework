SETUP_TESTS(
    test_storage
    test_command_events
    test_storage_notification
    test_test_vein_server
    test_storage_future_notification
)

target_sources(test_storage
  PRIVATE
    test-data/test-data.qrc
)

target_sources(test_command_events
  PRIVATE
    test-data/test-data.qrc
)

target_sources(test_test_vein_server
  PRIVATE
    test-data/test-data.qrc
)
