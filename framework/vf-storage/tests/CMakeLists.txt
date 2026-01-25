SETUP_TESTS(
    test_storage
    test_storage_notification
    test_storage_future_notification
    test_client_storage_event_system
    test_storage_direct_write
    test_storage_recorder
)

target_sources(test_storage
  PRIVATE
    test-data/test-data.qrc
)

target_sources(test_client_storage_event_system
  PRIVATE
    test-data/test-rpc.qrc
)

target_sources(test_storage_direct_write
  PRIVATE
    test-data/test-data.qrc
)

target_sources(test_storage_recorder
  PRIVATE
    test-data/test-data.qrc
)
