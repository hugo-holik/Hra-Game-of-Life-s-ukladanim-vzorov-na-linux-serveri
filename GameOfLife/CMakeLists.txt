cmake_minimum_required(VERSION 3.18)
project(GameOfLife C)

set(CMAKE_C_STANDARD 99)

add_executable(GameOfLife main.c Bunka.c Bunka.h Displej.c Displej.h Hra.c Hra.h CitacVzorov.c CitacVzorov.h Vzor.c Vzor.h arraylist.c arraylist.h server.c server.h client.c client.h)
target_link_libraries(GameOfLife pthread)
target_link_libraries(GameOfLife -lncurses)