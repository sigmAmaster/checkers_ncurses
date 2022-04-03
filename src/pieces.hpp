
#ifndef PIECES_HPP_INCLUDED
#define PIECES_HPP_INCLUDED

#include"game_board.hpp"
#include<map>
extern game_board main_bord;

// this object check all movement and actions for any piece and any type
// any action must be pass from this object 
class pieces{
    
    public:
    
    typedef const char* block_t;

    friend class ScreeN;
    friend class AI;
         
    pieces(){ //constructor
        
        piece_shape = nullptr;
        pieceX  = 0;
        pieceY  = 0;
        piece_selection = false;

      left_up = nullptr, right_up= nullptr,
      left_up_plus = nullptr, right_up_plus = nullptr,

      left_down = nullptr, right_down= nullptr,
      left_down_plus = nullptr, right_down_plus = nullptr;
              
      far_switch = false;
    }
    
    bool verfy_piece(){ //vrefy for choose correct pieces for move
        if ( piece_selection == false ){

        if (  *piece_shape == '@'  ||  *piece_shape == 'H' ){
                piece_selection = true;
                return true;
            }             
            else
            return false; 
        }
        
        else
        return false;
    } 

    void movment_calculate(); // calculate
   
    private:
    
    void data_filler( block_t , block_t );
    void king_movement();
    inline void loop_breaker(int, void (pieces::*that_func)( void ));
    int pieceX,pieceY;
    block_t piece_shape;

    // that means user is selected a piece for move
    bool piece_selection;  
    
    // for unlimited call for function movment and movment king
    bool far_switch;
    
    const std::array<std::array<char,max_bord_y>,max_bord_x>&play_ground = main_bord.brdige();
    
    std::vector< block_t > movement;
    std::map< block_t, block_t > deleted_pieces; 
    std::map< block_t, block_t> movment_chain;
    
    block_t // movment direction  
      
     left_up, right_up,
     left_up_plus, right_up_plus,

     left_down, right_down,
     left_down_plus, right_down_plus;
};


#endif // PIECES_HPP_INCLUDED
