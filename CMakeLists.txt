cmake_minimum_required(VERSION 3.0)

project(GameOfLife)

find_package(Curses REQUIRED)
if(NOT CURSES_FOUND)
  message(SEND_ERROR "Curses not found.")
  return()
else()
  include_directories(${CURSES_INCLUDE_DIR})
endif()

add_definitions(-std=c++11)

include_directories(.)

set(COMMON_SOURCES game_field.cpp game_handler.cpp)
set(TARGET_SOURCES main.cpp view_handler.cpp)
file(GLOB TEST_SOURCES tests/*.cpp gtest/*.cc)
message("f ${CURSES_LIBRARIES}")

add_executable(GameOfLife ${COMMON_SOURCES} ${TARGET_SOURCES})

target_link_libraries(GameOfLife ${CURSES_LIBRARIES})

add_executable(GameOfLifeTests ${COMMON_SOURCES} ${TEST_SOURCES})

target_link_libraries(GameOfLifeTests ${CURSES_LIBRARIES} pthread)

