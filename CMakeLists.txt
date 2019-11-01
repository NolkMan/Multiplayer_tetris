cmake_minimum_required(VERSION 2.8.12)
project(mult_tetris)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Curses REQUIRED)
set(CMAKE_C_FLAGS "-std=c99")

#include_directories(${PROJECT_SOURCE_DIR})

add_executable(tetris client.c ncurses_utils.c tetris_game.c client_server_comm.c comm_utils.c pieces.c utils.c)
add_executable(server server.c server_utils.c client_queue.c comm_utils.c utils.c)
add_executable(utests tests.c comm_utils.c utils.c)

target_link_libraries(tetris curses)
