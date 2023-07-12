include_directories(${PROJECT_SOURCE_DIR}/src/base/)
include_directories(${PROJECT_SOURCE_DIR}/platform/mockcpp/include)
include_directories(${PROJECT_SOURCE_DIR}/platform)

add_executable(unittest_base
    unittest_base_debug.cpp
    unittest_base_ini_cfg.cpp
    ${PROJECT_SOURCE_DIR}/src/base/debug.c
    ${PROJECT_SOURCE_DIR}/src/base/ini_cfg.c
    )

target_link_libraries(unittest_base
  PRIVATE
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest.a
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest_main.a
    ${PROJECT_SOURCE_DIR}/platform/mockcpp/lib/libmockcpp.a
  )

# automatic discovery of unit tests
gtest_discover_tests(unittest_base
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )

