#include"display_UI.hpp"
 // note : more clean code and transfer to pieces
    
    void ScreeN::display(){
 
        if (mission_compelet == true)
        return;            

        enum colors { 
            computer_pieces= 1, human_pieces,
            white_blocks, black_blocks,
            king_computer_p, king_human_p,
            correct_way, selected_pieces
        };
        
        init_pair(computer_pieces, COLOR_RED, COLOR_BLACK);
        init_pair(human_pieces, COLOR_CYAN, COLOR_BLACK); 

        init_pair(white_blocks, COLOR_WHITE, COLOR_WHITE); 
        init_pair(black_blocks, COLOR_BLACK, COLOR_BLACK);

        init_pair(king_computer_p, COLOR_YELLOW, COLOR_BLACK); 
        init_pair(king_human_p, COLOR_MAGENTA, COLOR_BLACK); 

        init_pair(correct_way, COLOR_GREEN, COLOR_GREEN); 
        init_pair(selected_pieces, COLOR_BLACK, COLOR_GREEN);
        
        char const* output = nullptr;
        int color_switcher=0;

        auto selectPiece = [&](){// color for selected piece by human
            
            if (output == OBJ.piece_shape && OBJ.piece_selection == true){
                wattron(board_screen, COLOR_PAIR(selected_pieces));
                color_switcher = selected_pieces;
            }
                        
            else{
                wattron(board_screen, COLOR_PAIR(human_pieces));
                color_switcher = human_pieces;
            }
        };
        
        wclear(board_screen);
        wrefresh(board_screen);

        for (size_t y = 0; y < max_bord_y; y++){

            for (size_t x = 0; x < max_bord_x; x++){
                
                output = &play_ground[y][x];

                switch (*output){ // fill block with correct color
        
                    case '*': wattron( board_screen, COLOR_PAIR(computer_pieces)); 
                    color_switcher = computer_pieces; break;

                    case '@':selectPiece();break;

                    case ' ': wattron( board_screen, COLOR_PAIR(white_blocks));
                    color_switcher = white_blocks;break;

                    case '#': wattron( board_screen, COLOR_PAIR(black_blocks));
                    color_switcher = black_blocks;break;

                    case 'H': selectPiece();break;

                    case 'C': wattron( board_screen, COLOR_PAIR(king_computer_p));
                    color_switcher = king_computer_p;break;

                    default: 
                        wclear(board_screen);
                        wprintw(board_screen,"system color error please report to programer\n ( press enter to exit )\n ");
                        
                        getch();
                        delwin(board_screen);delwin(stdscr);
                        exit_curses(0);
                }
                
                if ( !OBJ.movement.empty()){
                    /* 
                        if any piece has been selected and can move
                        this can chage blocks color for show correct move way  
                    */
                  
                    for (auto i:OBJ.movement ){ 
                        //check all saved blocks address for select correct blocks
                      
                       if (i == output ){
                            wattron( board_screen, COLOR_PAIR(correct_way));
                            color_switcher = correct_way;
                        }   
                    }
                
                }
                
                mvwaddch(board_screen,(y+1), (x+1),*output);// add block on screen
                wattroff(board_screen,COLOR_PAIR(color_switcher));
                // turn off color
          }   
         
        }
        
        // draw border
        box(board_screen,0,0);
        
        // refresh windows
        wrefresh(board_screen);

          
        
        
        
        mouse_ui();
        display();
    }

// this function is connection between desktop & game board data 

void ScreeN::mouse_ui(){

    keypad(board_screen, TRUE);      

     // *OBJ process human behavior for game bord data 

    MEVENT pos;
    int event = 0;
    
   auto get_mouse_local = [&](){
       bool click_is_ok = false;
    
        while ( click_is_ok != true ){
            
            event = wgetch(board_screen);
            
            // check to get key mouse
            if (event == KEY_MOUSE){
        
                // check everything is ok
                if (getmouse(&pos) == OK){
        
                    if ( pos.bstate & BUTTON1_CLICKED ){
                
                        /* so check left click mouse 
                        thats can be diffrent on 
                        other console app but this is 
                        a left click on xterm console
                        */
                       
                       // if user click in range
                       if ( ( pos.y && pos.y <= 20 )&& (pos.x>= 1 &&pos.x <=20 ) ) 
                        click_is_ok = true;
                    }
                }
    
                else      
                return;
            }
    
        }
    };

    if ( !OBJ.movement.empty() && OBJ.piece_selection == true ){ 
                    
        // if movment is not empty that means your pieces can move
        //check for selected block before do action
                    
        get_mouse_local();

        // if user click on piece again thats means user want to select other pieces
        if ( OBJ.piece_shape == &play_ground[(pos.y-1)][(pos.x-1)] ){
                        
            OBJ.piece_selection = false;
            OBJ.movement.clear(); 
           
            OBJ.deleted_pieces.clear();
            OBJ.piece_shape = nullptr;
                        
            return;
        }
                    
        // check wich block is cliked and it is correct block for move 
        else{
            
            for (auto &i :OBJ.movement) {
                        
                if (i == &play_ground[(pos.y-1)][(pos.x-1)]){// if address are equal

                    // (i) is current local piece                 

                    if ( !OBJ.deleted_pieces.empty() ){// we can delete enemy piece
                                
                        auto itr = OBJ.deleted_pieces.find(i);

                        // are you found any pieces for deleted
                        if ( itr != OBJ.deleted_pieces.end()){
                            

                            for (auto ChainLoop = OBJ.movment_chain.rbegin(); ChainLoop != OBJ.movment_chain.rend();ChainLoop++){
                            
                                auto MV = OBJ.deleted_pieces.find( ChainLoop->first );
                                
                                main_bord.delete_data( MV->second, OBJ.piece_shape, ChainLoop->first, true );              
                                   OBJ.piece_shape = ChainLoop->first;
                            }  
                            
                            mission_compelet=true;
                        }
                        
                        else{
                         main_bord.delete_data(nullptr,OBJ.piece_shape,i,true);
                         mission_compelet=true;
                        }
                        
                    }

                    else{                    
                        main_bord.delete_data(nullptr,OBJ.piece_shape,i,true);
                        mission_compelet=true;
                    }
                    
                }
            }
        }
        // if pieces is move so user is select correct block for moving
        // remake *OBJect for reset data for other pieces
        // now it is turn computer for moving                            
        if ( !mission_compelet){
             error_massage = "select correct way for move\n";
            error_windows();
        }
        
        else      
        return;    
    }
    
    // if piece shape is not empty and before you have selected piece before thats means your pieces can't move 
        
    else if ( OBJ.movement.empty() && OBJ.piece_selection == true ){ 
                            
        error_massage = "your piece can't move, please select other piece";
        error_windows();

        OBJ.piece_shape = nullptr;
        OBJ.piece_selection = false;
    }

    else{      
            get_mouse_local();
            
            // if your not selected piece before
            OBJ.piece_shape = &play_ground[(pos.y-1)][(pos.x-1)];

            // check user for select correct piece       
            bool result = OBJ.verfy_piece();

            if (result == true){
                OBJ.pieceX = (pos.x-1);// fill correct postion
                OBJ.pieceY = (pos.y-1);
                            
                // go to process near area for piece 
                OBJ.movment_calculate();
                            
                // after process all moving area show them on screen
    
            } 
            
            // do it again            
            else
                OBJ.piece_shape = nullptr;
        }

        return;
}  


 

 void ScreeN::error_windows(){

    struct error_area{
        const size_t height=10,width=60,
        start_y=24,start_x = 0;
        WINDOW *win_error = newwin(  height, width, start_y, start_x  );
    };
    
    error_area windows;
    
    init_pair(27,COLOR_RED,COLOR_WHITE);
    init_pair(37,COLOR_YELLOW,COLOR_BLACK);

    wmove(windows.win_error,5,3);
    
    wattron(windows.win_error,COLOR_PAIR(27));
        
        wprintw( windows.win_error, error_massage.c_str());
            
    wattroff(windows.win_error,COLOR_PAIR(27));
    
    refresh();

    wrefresh(windows.win_error);

    wattron(windows.win_error,COLOR_PAIR(37));

    int curser_switch = 0;
    int loop_row = 0 , loop_col = 0;
    int total_time = 0;
    
    do {
        
        switch (curser_switch){
        
        case 0: {
            if (loop_col == 59)
            curser_switch=1;

            else{
                
                wmove(windows.win_error,loop_row,loop_col);
                waddch(windows.win_error,' ');
                loop_col++;
            }

        }break;

        case 1:{
            
            if (loop_row == 9)
            curser_switch = 2;
            
            else{
                wmove(windows.win_error,loop_row,loop_col);
                waddch(windows.win_error,' ');
            
                loop_row++;
            }

        }break;

        case 2:{
        
        if (loop_col == 0)
        curser_switch = 3;
        
        else{
            
            wmove(windows.win_error,loop_row,loop_col);
            waddch(windows.win_error,' ');        
            
            loop_col--; 
        }
        }break;

        case 3:{

            if (loop_row == 0)
            curser_switch = 0;
            
            else{
            
                wmove(windows.win_error,loop_row,loop_col);
                waddch(windows.win_error,' ');
            
                loop_row--;
            }

        }break;
        
        default:break;
        }
        
        wmove(windows.win_error,loop_row,loop_col);
        waddch(windows.win_error,'#');
        
        wrefresh(windows.win_error);
        napms(10);

        total_time++;
 
    }while( total_time != 500);
    
    wattroff(windows.win_error,COLOR_PAIR(37));
    
    wclear(windows.win_error);
    wrefresh(windows.win_error);
    delwin(windows.win_error);
    
    refresh();
   
    return;
}

