cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -Wall")
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(CMAKE_VERBOSE_MAKEFILE  true)

find_package(Boost 1.53
  COMPONENTS
    program_options
    date_time
  REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} ..)

add_executable(fristen
  fristen.cc
  feiertage.cc
  )
target_link_libraries(fristen ${Boost_LIBRARIES})
