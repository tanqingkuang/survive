include(FetchContent)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build) #设定输出目录
include_directories(${PROJECT_SOURCE_DIR}/platform/googletest/include)
include(GoogleTest)

# 以下为工程差异
add_executable(
    unit_tests
    example_add.cpp
    example_subtract.cpp
    )

target_link_libraries(unit_tests
  PRIVATE
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest.a
    ${PROJECT_SOURCE_DIR}/platform/googletest/lib/libgtest_main.a
  )

# automatic discovery of unit tests
gtest_discover_tests(unit_tests
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )

