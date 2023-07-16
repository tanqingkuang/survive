include_directories(${PROJECT_SOURCE_DIR}/src/base/)
include_directories(${PROJECT_SOURCE_DIR}/src/animal)
include_directories(${PROJECT_SOURCE_DIR}/src/map)
include_directories(${PROJECT_SOURCE_DIR}/src/rule)
include_directories(${PROJECT_SOURCE_DIR}/platform/mockcpp/include)
include_directories(${PROJECT_SOURCE_DIR}/platform)

add_executable(unittest_animal
    unittest_animal.cpp
    ${PROJECT_SOURCE_DIR}/src/animal/animal_all.c
    ${PROJECT_SOURCE_DIR}/src/animal/animal.c
    ${PROJECT_SOURCE_DIR}/src/base/debug.c
    ${PROJECT_SOURCE_DIR}/src/base/ini_cfg.c
)

target_link_libraries(unittest_animal
  PRIVATE
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest.a
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest_main.a
    ${PROJECT_SOURCE_DIR}/platform/mockcpp/lib/libmockcpp.a
)

# automatic discovery of unit tests
gtest_discover_tests(unittest_animal
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
)

