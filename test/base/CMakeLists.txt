include_directories(${PROJECT_SOURCE_DIR}/src/base/)

add_executable(
    unittest_base
    unittest_base.cpp
    ${PROJECT_SOURCE_DIR}/src/base/debug.c
    ${BASE_SRC_LIST}
    )

target_link_libraries(unittest_base
  PRIVATE
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest.a
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest_main.a
  )

# automatic discovery of unit tests
gtest_discover_tests(unittest_base
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )

