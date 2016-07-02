cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Client)

include_directories(include/)

add_definitions(-W -Wall -Wextra -std=c++14)

find_package(Qt5Widgets REQUIRED)

if (NOT Qt5Widgets_FOUND)
  message(FATAL_ERROR "Qt5Widgets not found !")
endif()

include_directories(${Qt5Widgets_INCLUDE_DIRS})
file(GLOB_RECURSE client_srcs src/*.cpp)
add_executable(client ${client_srcs})
set_target_properties(
  client
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  )
target_link_libraries(client Qt5::Widgets)