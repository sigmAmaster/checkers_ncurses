#include"label.hpp"
#include<ncurses.h>

void logo_anime( void ){

initscr();
curs_set(0);
noecho();
cbreak();

 if(NCURSES_MOUSE_VERSION<0){
    printw("your terminal can't support mouse :(\n press enter for exit\n");
    getch();
    exit_curses(0);
 }

  
  if(!has_colors()){
    printw("your terminal can't support color :(\n");
    getch();
    exit_curses(0);
 }
   
  else{
        
    if ( start_color() != OK){
      printw("error in start terminal color \n press enter for exit\n");
      getch();
      exit_curses(0);
    }
        
  }

  clear();

  WINDOW *logo_win = newwin( 20, 41, ((LINES-20)/2), ((COLS-41)/2) );
  box(logo_win,0,0);
  refresh();


  start_color();

  init_pair(1,COLOR_RED,COLOR_BLACK);
  init_pair(2,COLOR_BLUE,COLOR_BLACK);
  
  init_pair(3,COLOR_WHITE,COLOR_BLACK);
  init_pair(4,COLOR_YELLOW,COLOR_BLACK);
  
  init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(6,COLOR_GREEN,COLOR_BLACK);
  
  init_pair(7,COLOR_CYAN,COLOR_BLACK);
  
  int counter [16] = 
  { 39, 39, 6, 6, 6, 6, 6, 8, 8, 6, 6, 6, 6, 6, 39, 39 };
//  0   0   0  2  4  6  8  10 10 8  6  4  2  0  0    0 

  int color_picker = 1;
  int space = 0;
  bool cursor_switch = true;

    for (size_t i = 0; i < 16; i++){

      int line_size = counter[i];

      if (cursor_switch){
       
        for (size_t l = 1; l <= line_size; l++){

          wattron(logo_win,COLOR_PAIR(color_picker) );
            mvwaddch( logo_win, (i+1), (l+space), '#');
          wattroff(logo_win, COLOR_PAIR(color_picker));
          
          wrefresh(logo_win);
          napms(10);
        }

        cursor_switch = false;
      }
      
      else{
        
        for (size_t l = line_size; l > 0; --l){

          wattron(logo_win,COLOR_PAIR(color_picker) );
            mvwaddch( logo_win, (i+1), (l+space), '#');
          wattroff(logo_win, COLOR_PAIR(color_picker));
          
          wrefresh(logo_win);
          napms(10);
        }

        cursor_switch = true;
      }
      
      if ( color_picker == 7 )
          color_picker = 1;
  
      else
        color_picker++;


      if ( (i >= 2) && (i < 7) )
        space += 2;
      
      else if ( (i >= 8) && (i <= 12)  )
        space -=2;  
    }

  mvwprintw(logo_win, 18,10,"Made By sigmAmaster");
  wrefresh(logo_win);

  napms(1200);
    
  wclear(logo_win);
  wrefresh(logo_win);

  delwin(logo_win);  
  
  clear();
  refresh();

    

  return;

}
