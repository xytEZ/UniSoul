cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Client)

include_directories(
  include/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/protocol/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/serializable/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/tool/
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/packet/
  )

add_definitions(-W -Wall -Wextra -std=c++14)

find_package(Boost 1.55.0 REQUIRED system serialization)

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
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/serializable/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/tool/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/packet/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../common/protocol/*.cpp
  )

add_executable(client ${client_srcs})
set_target_properties(
  client
  PROPERTIES
  COMPILE_FLAGS
  -lboost_system
  )
set_target_properties(
  client
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  )
target_link_libraries(client Qt5::Widgets ${Boost_LIBRARIES})