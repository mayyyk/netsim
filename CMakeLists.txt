cmake_minimum_required(VERSION 3.10)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Rygoristic flags
add_compile_options(-Wall -Wextra -pedantic -Werror)

# Logic library - removes the need to compile twice for tests and program run
set(SOURCE_FILES
    src/factory.cpp
    src/helpers.cpp
    src/nodes.cpp
    src/package.cpp
    src/reports.cpp
    src/simulation.cpp
    src/storage_types.cpp
    # excluding main.cpp and tests
)

add_library(netsim_lib ${SOURCE_FILES})

target_include_directories(netsim_lib PUBLIC include)

# main app for run

add_executable(netsim_app src/main.cpp)

target_link_libraries(netsim_app netsim_lib)

# tests

find_package(GTest CONFIG REQUIRED)

add_executable(netsim_tests test/main_gtest.cpp) 

target_link_libraries(netsim_tests netsim_lib GTest::gtest GTest::gtest_main)