cmake_minimum_required(VERSION 3.16)
project(checkers VERSION 1.0.0)

add_library(
    AI
    
    src/AI.cpp
    src/AI.hpp
)

add_library(
    piece
    
    src/pieces.hpp
    src/pieces.cpp
)

add_library(
    board
    
    src/game_board.cpp
    src/game_board.hpp
)

add_library(
    label
    
    src/label.cpp
    src/label.hpp
)

add_library(
    UI
    
    src/display_UI.cpp
    src/display_UI.hpp

)

add_library(
    ncurses
    SHARED
    IMPORTED
)
add_library(
    thread
    SHARED
    IMPORTED
)


add_executable(checkers src/main.cpp)

set_property(TARGET ncurses PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libncurses.so) // library path
set_property(TARGET thread PROPERTY IMPORTED_LOCATION  /usr/lib/x86_64-linux-gnu/libpthread.so) // library path

target_link_libraries(checkers PRIVATE AI UI label piece board ncurses thread)

