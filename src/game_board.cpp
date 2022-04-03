#include"game_board.hpp"

game_board main_bord;

void game_board::delete_data(const char* enemyPiece, const char* yourPiece, const char* destination,bool turn){
    
    char * topY = &game_bord[0][0];
    char * downY = &game_bord[19][0];

    // if turn true means human turn
    if (turn == true){
        
        auto itr2 = std::find(emptyBlocks.begin(),emptyBlocks.end(),destination);// find movment block in empty block list
        auto itr1 = std::find(humanPieces.begin(),humanPieces.end(),yourPiece);// find human piece in human piece list

        if ( itr1 != humanPieces.end() && itr2 != emptyBlocks.end() ){// if human piece and movment block are find
            **itr2 = *yourPiece; // fill empty block with human char
            **itr1 = '#'; // clean human piece location with '#'
            
            // if your Your piece new location on top y , then your piece can change to king pice
            for(int count = 0 ; count <= 18; count++){
                topY ++; 
                
                if(*itr2 == topY)
                    **itr2  = 'H';
            }

            emptyBlocks.push_back(*itr1); // add new location to empty list
            humanPieces.erase(itr1); // delete old location of human piece

            humanPieces.push_back(*itr2); // add new location to human list
            emptyBlocks.erase(itr2); // delete empty data becuase we are replace it with human piece
        }

        else// error   
        exit(EXIT_FAILURE);
        
        // if human movment can delete enemy
        if(enemyPiece != nullptr ){

            auto itr3 = std::find(computerPieces.begin(),computerPieces.end(),enemyPiece); // find human enemy piece in computer list

            **itr3 = '#'; // fill enemy location with empty char

            emptyBlocks.push_back(*itr3); // save location in empty
            computerPieces.erase(itr3); // delete enemy piece on list

            computer_pCounter--; // descrease piece counter
        }
           
    }
    
    else{

        // like human but all revers for computer
        auto itr1 = std::find(computerPieces.begin(),computerPieces.end(),yourPiece);
        auto itr2 = std::find(emptyBlocks.begin(),emptyBlocks.end(),destination);

        if (itr1 != computerPieces.end() && itr2 != emptyBlocks.end()){
            **itr2 = *yourPiece;
            **itr1 = '#';

            for(int count = 0 ; count <= 18; count++){
                downY ++; 
                
                if(*itr2 == downY)
                 **itr2  = 'C';
            }



            emptyBlocks.push_back(*itr1);
            computerPieces.erase(itr1);

            computerPieces.push_back(*itr2);
            emptyBlocks.erase(itr2);
           
        }

        else
        exit(EXIT_FAILURE);


        if(enemyPiece != nullptr ){

            auto itr3 = std::find(humanPieces.begin(),humanPieces.end(),enemyPiece);

            **itr3 = '#';
            emptyBlocks.push_back(*itr3);
            humanPieces.erase(itr3);

            human_pCounter--;
        }
    
             
    }
    
    return;
}

int game_board::find_location_pice(const char* searchThis,bool YorX){

    int pos = 0;

    for ( auto y = 0; y < max_bord_y; y++){

        for (auto x = 0; x < max_bord_x; x++){

            if ( searchThis == &game_bord[y][x] ){
                if (YorX == true)
                return y;

                else
                return x;
            }
        }

    }
    exit(EXIT_FAILURE);
}
