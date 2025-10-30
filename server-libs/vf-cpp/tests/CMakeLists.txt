SETUP_TESTS(
    test_rpc_signature
    test_rpc_simplified
)

target_sources(test_rpc_simplified
  PRIVATE
    test-data/test-data.qrc
)
