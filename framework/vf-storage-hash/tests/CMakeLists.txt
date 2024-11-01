SETUP_TESTS(
    test_storage
    test_storage_notification
    test_storage_future_notification
)

target_sources(test_storage
  PRIVATE
    test-data/test-data.qrc
)

