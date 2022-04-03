 #include<array>
#include<vector>
#include<algorithm>
#include<ncurses.h>

#ifndef GAME_BOARD_HPP_INCLUDED
#define GAME_BOARD_HPP_INCLUDED

#define max_bord_y 20
#define max_bord_x 20
#define LocalSize 2
    class game_board{

    public:     

        game_board(){
            
            computer_pCounter = 0,
            human_pCounter = 0;
            
         game_bord =  {
                {

                    {' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*'} // ' ' =white block  // @ = human piece >>i=0                   
                    ,
                    {'*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' '} // # =black block   // * = computer piece                          
                    ,
                    {' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*'}
                    ,
                    {'*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' '}
                    ,
                    {' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*',' ','*'}
                    ,
                    {'#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' '}       
                    ,
                    {' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#'}           
                    ,
                    {'#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' '}       
                    ,
                    {' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#'}           
                    ,
                    {'#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' '}       
                    ,
                    {' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#'}           
                    ,
                    {'#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' '}       
                    ,   
                    {' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#'}           
                    ,
                    {'#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' '}       
                    ,
                    {' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#',' ','#'}           
                    ,
                    {'@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' '}
                    ,
                    {' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@'}
                    ,
                    {'@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' '}
                    ,
                    {' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@'}   
                    ,
                    {'@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' ','@',' '}
                    }
        
                };


            for ( int y = 0; y < max_bord_y; y++){
                
               for (auto x = 0 ; x < max_bord_x; x++){
                
                    if ( game_bord[y][x] == '*' || game_bord[y][x] == 'C'){
                        computerPieces.push_back( &game_bord[y][x] );
                        computer_pCounter++;
                    }
                    
                    else if( game_bord[y][x] == '@' || game_bord[y][x] == 'H'){
                        humanPieces.push_back( &game_bord[y][x] );
                        human_pCounter++;
                    }
                    
                    else if ( game_bord[y][x] == '#'){
                        emptyBlocks.push_back( &game_bord[y][x] );
                    }
                }   
            }
            
        }   
    
        const std::array<std::array<char,max_bord_y>,max_bord_x>& brdige() const{ return game_bord ; };
        /*
        
        std::array<std::array<char,max_bord_y>,max_bord_x>& copy_maker(){
            std::array<std::array<char,max_bord_y>,max_bord_x> copy_bord = game_bord;
            return copy_bord;
        }
        */
        
        int find_location_pice(const char* searchThis,bool);

        const std::vector<char*>& GetPieceData(const int selector) const{
         
           switch (selector){

           case 1: return computerPieces; break;
           case 2: return humanPieces; break;
           case 3: return emptyBlocks; break;

           default: exit(EXIT_FAILURE);break;
           }   
        }
        
        int GameResult(){
           
           if ( computer_pCounter == 0 )
            return 3;
           
           else if( human_pCounter == 0)
            return 2;
           
           else
           return 1;
       }
        // delete and replace data in bord *OBJect
        void delete_data(const char* enemyPiece, const char* yourPiece, const char* destination,bool turn);
        
        private:
        std::vector<char*> computerPieces;
        std::vector<char*> humanPieces;
        std::vector<char*> emptyBlocks;
        int computer_pCounter,human_pCounter;

        std::array< std::array<char, max_bord_y>, max_bord_x > game_bord;

    };
    
extern game_board main_bord;

#endif // GAME_BOARD_HPP_INCLUDED
