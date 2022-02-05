cmake_minimum_required(VERSION 2.8...3.22.1)
project(brwahl CXX)

find_package(Boost 1.53
  COMPONENTS
    program_options
    date_time
  REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(fristen
  fristen.cc
  feiertage.cc
  dhondt.cc
  mitglieder.cc
  )
target_link_libraries(fristen ${Boost_LIBRARIES})
