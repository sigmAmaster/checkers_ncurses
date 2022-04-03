#include"label.hpp"
#include"game_board.hpp"
#include"display_UI.hpp"
#include"AI.hpp"

void setting();
void menu();
void user_help();
inline void score();
inline bool check_end_of_game();
void END_massage( std::string txt,int loseORwin);

int main(){

    logo_anime();
    
    menu();
}

void menu(){
    
    mousemask(ALL_MOUSE_EVENTS,NULL);
    keypad(stdscr, TRUE);      

    init_pair(23,COLOR_RED,COLOR_BLACK);
    init_pair(45,COLOR_WHITE,COLOR_BLUE);

    clear();

    wattron(stdscr,COLOR_PAIR(23));
        printw("WELCOME TO CHECKERS");
    wattroff(stdscr,COLOR_PAIR(23));
    
    wattron(stdscr,COLOR_PAIR(45) );
     mvprintw(2,1,"Start Game\n");
     mvprintw(4,1,"Help\n");
     mvprintw(6,1,"EXIT");
    wattroff(stdscr,COLOR_PAIR(45));

    refresh();
    MEVENT MOUSE;
    
    keypad(stdscr, TRUE);      

    while ( wgetch(stdscr) ){
       
        getmouse(&MOUSE);

        if (MOUSE.bstate & BUTTON1_CLICKED){
            
            switch (MOUSE.y){
            
            case 2:{ clear(); refresh(); setting(); }break;
            
            case 4: user_help();break;
            
            case 6:{ 
                 
                 clear(); refresh(); 
                 
                 wattron(stdscr,COLOR_PAIR(23));
                    mvprintw(0,0,"GOOD BYE !!!");
                    refresh();
                 wattroff(stdscr,COLOR_PAIR(23));
                 
                 napms(3000);

                 clear(); refresh(); 
                 
                 exit(EXIT_SUCCESS); 
                } break;
            
                default:continue;
            }
        }
        
    }
    
}

void user_help(){

struct help_window{
 const size_t height=25,width=60,
 start_y=20,start_x=0;
 WINDOW *win_help = newwin(  height, width, start_y, start_x  );
};

help_window win;

    wprintw(win.win_help,"\nhelp : \n your normal piece: @\n your king piece: H\n computer normal piece: *\n computer king piece: C");

    wprintw(win.win_help,"\n you can select your piece by left click on mouse \n");

    wprintw(win.win_help," and select your move way by click on green block,");

    wprintw(win.win_help,"\n if you want select other piece you can \n click again on selected piece\n");
    
    wprintw(win.win_help," and then select other piece\n");

    wprintw(win.win_help," (perss any key for exit help menu)\n");
    
    refresh();
    wrefresh(win.win_help);
    getch();

    wclear(win.win_help);
    wrefresh(win.win_help);

    delwin(win.win_help);
    refresh();

    return;
}

void setting(){
    
    init_pair(11,COLOR_BLACK,COLOR_GREEN);// win background, human score color
    init_pair(12,COLOR_BLACK,COLOR_RED); // lose background, computer score color

    // show game result in window and close prgram
   
    auto human_turn = [&](){ ScreeN manitor; manitor.display(); };
    auto computer_turn = [&](){ AI computer; computer.decision(); };
    
    /// Game Flow
    while (true){
        
        score();
        human_turn();
        if ( check_end_of_game() ) break;

        score();
        computer_turn();
        if( check_end_of_game() ) break;
    
    }        
    
    exit(EXIT_SUCCESS); // end of the game
     
}

inline bool check_end_of_game(){
    
    int anwser = main_bord.GameResult();// get result game

    switch (anwser){

        case 3:END_massage("YOU WIN CONGRATS :)",11); return true; break;// human win

        case 2:END_massage("YOU LOSE TRY HARDER :(",12); return true; break;// computer win

        default:return false; break;
    }
}




inline void score (){
    
    int humanP = main_bord.GetPieceData(2).size();
    int computerP = main_bord.GetPieceData(1).size();
        
    std::string H = "your piece left: ";
    H += std::to_string(humanP);

    std::string C = "computer piece left: ";
    C += std::to_string(computerP);
        
    wattron(stdscr,COLOR_PAIR(11));
       mvprintw(0,23,H.c_str());
    wattroff(stdscr,COLOR_PAIR(11));

    wattron(stdscr,COLOR_PAIR(12));        
        mvprintw(2,23,C.c_str());
    wattroff(stdscr,COLOR_PAIR(12));

    refresh();
    return;
}


void END_massage( std::string txt,int loseORwin){
    

    struct result_windows{
        const size_t
        hieght=30,width=30,
        startY=0,startX=0;

     WINDOW *res_win = newwin(hieght,width,startY,startX);
    };

    result_windows show;
    napms(3000);
    clear();

    wattron(show.res_win,COLOR_PAIR(loseORwin));
        wborder(show.res_win,'*','*','M','W','#','#','#','#');
            wmove(show.res_win,15,5);
                wprintw(show.res_win,txt.c_str());
     wattroff(show.res_win,COLOR_PAIR(loseORwin));

    refresh();
    wrefresh(show.res_win);
    napms(5000);
    delwin(show.res_win);

    return;
}