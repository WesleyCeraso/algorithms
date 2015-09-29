cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.12.12.tar.gz"
    SHA1 "4c1ea8c6c576c5d898065f72ea03308b8dd853f7"
)

project(Algorithms)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED NO_CMAKE_BUILDS_PATH)

file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

file(GLOB_RECURSE UNIT_TEST_HEADERS "test/unit/*.h")
file(GLOB_RECURSE UNIT_TEST_SOURCES "test/unit/*.cpp")

file(GLOB_RECURSE FUNC_TEST_HEADERS "test/functional/*.h")
file(GLOB_RECURSE FUNC_TEST_SOURCES "test/functional/*.cpp")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=gnu++0x -Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(${CMAKE_SOURCE_DIR})
add_library(algorithms ${HEADERS} ${SOURCES})

add_executable(algorithms_unit ${UNIT_TEST_HEADERS} ${UNIT_TEST_SOURCES} )
target_link_libraries(algorithms_unit algorithms GTest::main)

add_executable(algorithms_functional ${FUNC_TEST_HEADERS} ${FUNC_TEST_SOURCES} )
target_link_libraries(algorithms_functional algorithms GTest::main)

enable_testing()
add_test(UnitTests algorithms_unit)
add_test(FunctionalTests algorithms_functional)
