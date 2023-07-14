include_directories(${PROJECT_SOURCE_DIR}/src/base/)
include_directories(${PROJECT_SOURCE_DIR}/src/map)
include_directories(${PROJECT_SOURCE_DIR}/platform/mockcpp/include)
include_directories(${PROJECT_SOURCE_DIR}/platform)

add_executable(unittest_map
    unittest_map.cpp
    ${PROJECT_SOURCE_DIR}/src/base/debug.c
    ${PROJECT_SOURCE_DIR}/src/base/ini_cfg.c
    ${PROJECT_SOURCE_DIR}/src/map/map.c
    ${PROJECT_SOURCE_DIR}/src/map/map_data.c
    ${PROJECT_SOURCE_DIR}/src/map/map_ini_cfg.c
    )

target_link_libraries(unittest_map
  PRIVATE
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest.a
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest_main.a
    ${PROJECT_SOURCE_DIR}/platform/mockcpp/lib/libmockcpp.a
  )

# automatic discovery of unit tests
gtest_discover_tests(unittest_map
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )

