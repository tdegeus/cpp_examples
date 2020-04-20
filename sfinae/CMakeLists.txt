project(main)

cmake_minimum_required(VERSION 3.16)

set(HIGHFIVE_USE_XTENSOR 1)
set(HIGHFIVE_USE_EIGEN 1)
set(HIGHFIVE_USE_BOOST 0)
find_package(HighFive)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE HighFive)
