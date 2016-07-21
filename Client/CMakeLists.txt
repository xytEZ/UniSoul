cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Client)

include_directories(
  include/
  ${CMAKE_CURRENT_SOURCE_DIR}/../other/protocol/
  ${CMAKE_CURRENT_SOURCE_DIR}/../other/serializable/
  ${CMAKE_CURRENT_SOURCE_DIR}/../other/tool/
  )

add_definitions(-W -Wall -Wextra -std=c++14)

find_package(Boost 1.55.0 REQUIRED serialization)

if (NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found !")
endif()


find_package(Qt5Widgets REQUIRED)

if (NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Qt5Widgets not found !")
endif()

include_directories(${Qt5Widgets_INCLUDE_DIRS})

file(
  GLOB_RECURSE
  client_srcs
  src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../other/serializable/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../other/tool/*.cpp
  )

add_executable(client ${client_srcs})
set_target_properties(
  client
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  )
target_link_libraries(client Qt5::Widgets ${Boost_Libraries})