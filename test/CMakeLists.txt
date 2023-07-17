include(FetchContent)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build) #设定输出目录
include_directories(${PROJECT_SOURCE_DIR}/platform/googletest/include)
include(GoogleTest)

add_subdirectory(base)
add_subdirectory(map)
add_subdirectory(animal)
add_subdirectory(rule)