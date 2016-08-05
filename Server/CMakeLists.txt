cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Server)

include_directories(
  include/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/protocol/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/serializable/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/tool/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/packet/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/other/
  )

add_definitions(-W -Wall -Wextra -std=c++14)

find_package(Boost 1.55.0 REQUIRED system serialization)

if (NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found !")
endif()

file(
  GLOB_RECURSE
  server_srcs
  src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/serializable/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/tool/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/packet/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/protocol/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/other/*.cpp
  )

add_executable(server ${server_srcs})
set_target_properties(
  server
  PROPERTIES
  COMPILE_FLAGS
  -lboost_system
  )
set_target_properties(
  server
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  )
target_link_libraries(server ${Boost_LIBRARIES})