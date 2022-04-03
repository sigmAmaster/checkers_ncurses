
#ifndef DISPLAY_UI_HPP_INCLUDED
#define DISPLAY_UI_HPP_INCLUDED

#include"pieces.hpp"
#include"game_board.hpp"

extern game_board main_bord;

class ScreeN{

friend class pieces;

public:

ScreeN():
OBJ(), play_ground(main_bord.brdige()){ mission_compelet = false; board_screen = win.win_board; };

void display();
private:
 
 struct board_area{
        const size_t height=22,width=22,
        start_y=0,start_x=0;
        WINDOW *win_board = newwin(  height, width, start_y, start_x  );
    };

 board_area win;

void mouse_ui();
void error_windows();
WINDOW *board_screen;

bool mission_compelet;
const std::array<std::array<char,max_bord_y>,max_bord_x>&play_ground;
pieces OBJ;

std::string error_massage;

};



#endif // DISPLAY_UI_HPP_INCLUDED
