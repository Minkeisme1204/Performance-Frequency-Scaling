cmake_minimum_required(VERSION 3.10)
project(CPUFeaturesLogger C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(cpu_info_logger STATIC include/cpu_info_logger.c)

add_executable(logger src/logger.c)
target_link_libraries(logger cpu_info_logger)

add_executable(collect src/collect_data.c)
target_link_libraries(collect cpu_info_logger)